#include <BoolCalc.h>

#include "Node.h"
#include "BoolExprParser.h"

#include <climits>
#include <stack>

#include <boost/spirit/include/qi.hpp>
#include <boost/lexical_cast.hpp>

struct TreeInfo
{
	std::size_t numberOfEpxr;
	std::size_t numberOfSubsituationVars;
	std::size_t numberOfVars;
};


void visitNode(const std::shared_ptr<bcc::Node> &node, TreeInfo &info)
{
	const std::shared_ptr<bcc::Var> pvar = std::dynamic_pointer_cast<bcc::Var>(node);
	if(pvar)
	{
		if(pvar -> m_varId > info.numberOfVars)
			info.numberOfVars = pvar -> m_varId;
		info.numberOfSubsituationVars++;
	}
	else if(std::dynamic_pointer_cast<bcc::Expr>(node))
		info.numberOfEpxr++;
	else
		info.numberOfEpxr++;			

	std::list<std::shared_ptr<bcc::Node> >::const_iterator itr = node -> m_childs.begin();

	for(;itr != node -> m_childs.end(); itr++)
		visitNode(*(itr), info);
}


TreeInfo getInfo(const std::shared_ptr<bcc::Node> &node)
{
	TreeInfo result;
	result.numberOfVars = 0;
	result.numberOfEpxr = 0;
	result.numberOfSubsituationVars = 0;
	visitNode(node, result);
	return result;
}


void processNodeForRPN(const std::shared_ptr<bcc::Node> &node, void **pdata, std::size_t type)
{
	std::list<std::shared_ptr<bcc::Node> >::const_iterator itr = node -> m_childs.begin();

	for(;itr != node -> m_childs.end(); itr++)
		processNodeForRPN(*itr, pdata, type);

	const std::shared_ptr<bcc::Expr> expr = std::dynamic_pointer_cast<bcc::Expr>(node);
	const std::shared_ptr<bcc::Var> var = std::dynamic_pointer_cast<bcc::Var>(node);
	const std::shared_ptr<bcc::Const> const_ = std::dynamic_pointer_cast<bcc::Const>(node);



	if(expr)
	{
		unsigned char entry;
		entry = ((unsigned char)(expr -> m_exprType) + 2) | (1 << (CHAR_BIT - 1));
		*((unsigned char *) *pdata) = entry;
		*pdata = (unsigned char *) *pdata + 1;
	}
	else if(const_)
	{
		unsigned char entry;
		entry = ((unsigned char) const_ -> m_value) | (1 << (CHAR_BIT - 1));
		*((unsigned char *) *pdata) = entry;
		*pdata = (unsigned char *) *pdata + 1;
	}
	else
	{
		unsigned long long entry = var -> m_varId;

		switch(type)
		{
			case 1:
				*((unsigned char *) *pdata) = entry;
				*pdata = (unsigned char *) *pdata + 1;
				break;
			case 2:
				*((unsigned short *) *pdata) = entry;
				*pdata = (unsigned short *) *pdata + 1;
				break;
			case 3:
				*((unsigned int *) *pdata) = entry;
				*pdata = (unsigned int *) *pdata + 1;
				break;
			case 4:
				*((unsigned long long *) *pdata) = entry;
				*pdata = (unsigned long long *) *pdata + 1;
				break;
		};
	}
}


bcc::Function::Function(const std::string &expression):
	m_pimpl(NULL)
{
	std::string expr = expression;
	BoolExprParser<std::string::iterator> parser;
	std::string::iterator itr = expr.begin();
	bool res = boost::spirit::qi::parse(itr, expr.end(), parser);

	if(!res || itr != expr.end())
		throw std::runtime_error("expression `" + expression + "` is incorrect");

	m_root = parser.m_root;

	TreeInfo info = getInfo(m_root);

	if(info.numberOfVars > (ULLONG_MAX >> 1))
		throw std::runtime_error("number of variables (" + 
			boost::lexical_cast<std::string>(info.numberOfVars) +
			") is too big");

	char varSize;
	std::size_t type;

	if(((info.numberOfVars + 1) << 1) < UCHAR_MAX)
	{
		varSize = sizeof(unsigned char);
		type = 1;
	}
	else if(((info.numberOfVars + 1) << 1) < USHRT_MAX)
	{
		varSize = sizeof(unsigned short);
		type = 2;
	}
	else if(((info.numberOfVars + 1) << 1) < UINT_MAX)
	{
		type = 3;
		varSize = sizeof(unsigned int);
	}
	else
	{
		type = 4;
		varSize = sizeof(unsigned long long);
	}

	std::size_t exprBuffSize = info.numberOfSubsituationVars * varSize + 
		info.numberOfEpxr * sizeof(unsigned char);

	if(exprBuffSize > ULLONG_MAX)
		throw std::runtime_error("expression is too big");

	m_pimpl = malloc(exprBuffSize + 1 + sizeof(unsigned long long));
	*((char *) m_pimpl) = type;
	*(unsigned long long *)(((char *) m_pimpl) + 1) = (unsigned long long) exprBuffSize;

	void *pdata = ((unsigned long long *)((char *) m_pimpl + 1)) + 1;
	std::size_t position = 0;

	processNodeForRPN(parser.m_root, &pdata, type);

}


bcc::Function::~Function()
{
	if(m_pimpl)
		free(m_pimpl);
}


bool bcc::Function::calculate(const std::vector<bool> &values) const throw(std::runtime_error)
{
	if(!m_root)
		throw std::runtime_error("expression is empty");

	return m_root -> exec(values);
}


bool bcc::Function::calculate(const boost::dynamic_bitset<> &values) const throw(std::runtime_error)
{
	if(!m_root)
		throw std::runtime_error("expression is empty");

	return m_root -> exec(values);
}


bool bcc::Function::calculateRPN(const boost::dynamic_bitset<> &values) const throw(std::runtime_error)
{
	if(!m_pimpl)
		throw std::runtime_error("expression is empty");

	std::stack<bool> stack;

	std::size_t n = *((unsigned long long *)(((char *) m_pimpl) + 1));
	std::size_t type = *((char *) m_pimpl);

	void *pdata = ((unsigned long long *)((char *) m_pimpl + 1)) + 1;
	void *pdataEnd = (unsigned char *) pdata + n;

	for(;pdata != pdataEnd;)
	{	
		if(*((unsigned char *) pdata) & (1 << (CHAR_BIT - 1)))
		{
			unsigned char entry = *((unsigned char *) pdata) ^  (1 << (CHAR_BIT - 1));
			if(entry < 2)
				stack.push(entry);
			else
			{
				entry -= 2;
				if(entry == Expr::AND)
				{
					bool op2 = stack.top();
					stack.pop();
					bool op1 = stack.top();
					stack.pop();
					stack.push(op1 && op2);
				}
				else if(entry == Expr::OR)
				{
					bool op2 = stack.top();
					stack.pop();
					bool op1 = stack.top();
					stack.pop();
					stack.push(op1 || op2);
				}
				else if(entry == Expr::XOR)
				{
					bool op2 = stack.top();
					stack.pop();
					bool op1 = stack.top();
					stack.pop();
					stack.push(op1 != op2);
				}
				else if(entry == Expr::NOT)
				{
					bool op = stack.top();
					stack.pop();
					stack.push(!op);
				}
			}
			pdata = ((unsigned char *) pdata) + 1;
		}
		else
		{
			unsigned long long entry;

			switch(type)
			{
				case 1:
					entry = *((unsigned char *) pdata);
					pdata = ((unsigned char *) pdata) + 1;
					break;
				case 2:
					entry = *((unsigned short *) pdata);
					pdata = ((unsigned short *) pdata) + 1;
					break;
				case 3:
					entry = *((unsigned int *) pdata);
					pdata = ((unsigned int *) pdata) + 1;
					break;
				case 4:
					entry = *((unsigned long long *) pdata);
					pdata = ((unsigned long long *) pdata) + 1;
					break;
			};

			stack.push(values[entry]);
		}
	}

	if(stack.size() != 1)
		throw std::runtime_error("internal error");
	return stack.top();
}
