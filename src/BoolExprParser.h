#ifndef BOOL_EXPR_PARSER_H_
#define BOOL_EXPR_PARSER_H_

#include <memory>
#include <deque>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/bind.hpp>


#include "Node.h"
#include "Expr.h"
#include "Var.h"
#include "Const.h"

namespace bcc
{

template <typename Iterator>
class BoolExprParser: public boost::spirit::qi::grammar<Iterator>
{
	public:
		BoolExprParser():
		BoolExprParser::base_type(expression)
		{
			namespace qi = boost::spirit::qi;
			namespace phoenix = boost::phoenix;
			using phoenix::push_front;

			expression = *qi::space >> term >> *( 
                    (*qi::space >>  '|' >> *qi::space >> term) [boost::bind(&BoolExprParser::Or_, this)]
                    | 
                    (*qi::space >>	'+'  >> *qi::space >> term) [boost::bind(&BoolExprParser::Xor_, this)]
                );

			term       = *qi::space >> factor >> *( 
                    (*qi::space >> '&' >> *qi::space >> factor) [boost::bind(&BoolExprParser::And_, this)]
                    );

			factor      = element  | 
                        ('!' >> *qi::space >> element) [boost::bind(&BoolExprParser::Not_, this)];

			element     = (
                    qi::char_('0') [push_front(phoenix::ref(m_elementsQueue), 
                        std::shared_ptr<Node>(new Const(false)))]
                    |
                    qi::char_('1') [push_front(phoenix::ref(m_elementsQueue), 
                        std::shared_ptr<Node>(new Const(true)))]
                    |
                    ('x' >> qi::uint_[boost::bind(&BoolExprParser::Var_, this, _1)])        
                    | 
                    ('(' >> *qi::space >> expression >> *qi::space >> ')')
                )[boost::bind(&BoolExprParser::UpdateRoot, this)];
		}

		std::shared_ptr<Node>                 m_root;

		void clear()
		{
			m_elementsQueue.clear();
			m_root.reset();
		}

	private:
		void Var_(unsigned int n)
		{
			m_elementsQueue.push_front(std::shared_ptr<Node>(new Var(n)));
			UpdateRoot();
		}

		void And_()
		{
			std::shared_ptr<Node> op(new Expr(Expr::AND));

			op -> m_childs.push_front(m_elementsQueue.front());
			m_elementsQueue.pop_front();
			op -> m_childs.push_front(m_elementsQueue.front());
			m_elementsQueue.pop_front();

			m_elementsQueue.push_front(op);
			UpdateRoot();
		}

		void Or_()
		{
			std::shared_ptr<Node> op(new Expr(Expr::OR));

			op -> m_childs.push_front(m_elementsQueue.front());
			m_elementsQueue.pop_front();
			op -> m_childs.push_front(m_elementsQueue.front());
			m_elementsQueue.pop_front();

			m_elementsQueue.push_front(op);
			UpdateRoot();
		}

		void Xor_()
		{
			std::shared_ptr<Node> op(new Expr(Expr::XOR));

			op -> m_childs.push_front(m_elementsQueue.front());
			m_elementsQueue.pop_front();
			op -> m_childs.push_front(m_elementsQueue.front());
			m_elementsQueue.pop_front();

			m_elementsQueue.push_front(op);
			UpdateRoot();
		}

		void Not_()
		{
			std::shared_ptr<Node> op(new Expr(Expr::NOT));

			op -> m_childs.push_front(m_elementsQueue.front());
			m_elementsQueue.pop_front();

			m_elementsQueue.push_front(op);
			UpdateRoot();
		}

		void UpdateRoot()
		{
			m_root = m_elementsQueue.front();
		}

		boost::spirit::qi::rule<Iterator>     expression;
		boost::spirit::qi::rule<Iterator>     term;
		boost::spirit::qi::rule<Iterator>     factor;
		boost::spirit::qi::rule<Iterator>     element;

		std::deque<std::shared_ptr<Node> >    m_elementsQueue;
};

}

#endif
