#pragma once
#include "stdafx.h"

namespace with_state
{
	struct IState
	{
		virtual void InsertQuarter() = 0;
		virtual void EjectQuarter() = 0;
		virtual void TurnCrank() = 0;
		virtual void Dispense() = 0;
		virtual void Refill(unsigned ballsCount) = 0;
		virtual std::string ToString()const = 0;
		virtual ~IState() = default;
	};

	struct IGumballMachine
	{
		virtual void AddBalls(unsigned ballsCount) = 0;
		virtual void ReleaseBall() = 0;
		virtual void UseQuarter() = 0;
		virtual void AddQuarter() = 0;
		virtual void EjectQuarters() = 0;
		virtual unsigned GetBallCount()const = 0;
		virtual unsigned GetQuartersCount()const = 0;

		virtual void SetSoldOutState() = 0;
		virtual void SetNoQuarterState() = 0;
		virtual void SetSoldState() = 0;
		virtual void SetHasQuarterState() = 0;

		virtual ~IGumballMachine() = default;
	};

	class CSoldState : public IState
	{
	public:
		CSoldState(IGumballMachine & gumballMachine)
			:m_gumballMachine(gumballMachine)
		{}

		void InsertQuarter() override
		{
			std::cout << "Please wait, we're already giving you a gumball\n";
		}

		void EjectQuarter() override
		{
			std::cout << "Sorry you already turned the crank\n";
		}

		void TurnCrank() override
		{
			std::cout << "Turning twice doesn't get you another gumball\n";
		}

		void Dispense() override
		{
			m_gumballMachine.ReleaseBall();
			if (m_gumballMachine.GetBallCount() == 0)
			{
				std::cout << "Oops, out of gumballs\n";
				m_gumballMachine.SetSoldOutState();
			}
			else
			{
				if (m_gumballMachine.GetQuartersCount() == 0)
				{
					m_gumballMachine.SetNoQuarterState();
				}
				else
				{
					m_gumballMachine.SetHasQuarterState();
				}
			}
		}

		void Refill(unsigned ballsCount) override
		{
			m_gumballMachine.AddBalls(ballsCount);
		}

		std::string ToString() const override
		{
			return "delivering a gumball";
		}
	private:
		IGumballMachine & m_gumballMachine;
	};

	class CSoldOutState : public IState
	{
	public:
		CSoldOutState(IGumballMachine & gumballMachine)
			:m_gumballMachine(gumballMachine)
		{}

		void InsertQuarter() override
		{
			std::cout << "You can't insert a quarter, the machine is sold out\n";
		}

		void EjectQuarter() override
		{
			if (m_gumballMachine.GetQuartersCount() == 0)
			{
				std::cout << "You can't eject, you haven't inserted a quarter yet\n";
			}
			else
			{
				m_gumballMachine.EjectQuarters();
				m_gumballMachine.SetNoQuarterState();
			}
		}

		void TurnCrank() override
		{
			std::cout << "You turned but there's no gumballs\n";
		}

		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}

		void Refill(unsigned ballsCount) override
		{
			m_gumballMachine.AddBalls(ballsCount);
			if (m_gumballMachine.GetBallCount() == 0)
			{
				return;
			}
			if (m_gumballMachine.GetQuartersCount() > 0)
			{
				m_gumballMachine.SetHasQuarterState();
			}
			else
			{
				m_gumballMachine.SetNoQuarterState();
			}
		}

		std::string ToString() const override
		{
			return "sold out";
		}
	private:
		IGumballMachine & m_gumballMachine;
	};

	class CHasQuarterState : public IState
	{
	public:
		CHasQuarterState(IGumballMachine & gumballMachine)
			:m_gumballMachine(gumballMachine)
		{}

		void InsertQuarter() override
		{
			if (m_gumballMachine.GetQuartersCount() < 5)
			{
				m_gumballMachine.AddQuarter();
			}
			else
			{
				std::cout << "You can't insert another quarter\n";
			}
		}

		void EjectQuarter() override
		{
			std::cout << "Quarter returned\n";
			m_gumballMachine.EjectQuarters();
			m_gumballMachine.SetNoQuarterState();
		}

		void TurnCrank() override
		{
			std::cout << "You turned...\n";
			m_gumballMachine.UseQuarter();
			m_gumballMachine.SetSoldState();
		}

		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}

		void Refill(unsigned ballsCount) override
		{
			m_gumballMachine.AddBalls(ballsCount);
		}

		std::string ToString() const override
		{
			return "waiting for turn of crank";
		}
	private:
		IGumballMachine & m_gumballMachine;
	};

	class CNoQuarterState : public IState
	{
	public:
		CNoQuarterState(IGumballMachine & gumballMachine)
			: m_gumballMachine(gumballMachine)
		{}

		void InsertQuarter() override
		{
			std::cout << "You inserted a quarter\n";
			m_gumballMachine.AddQuarter();
			m_gumballMachine.SetHasQuarterState();
		}

		void EjectQuarter() override
		{
			std::cout << "You haven't inserted a quarter\n";
		}

		void TurnCrank() override
		{
			std::cout << "You turned but there's no quarter\n";
		}

		void Dispense() override
		{
			std::cout << "You need to pay first\n";
		}

		void Refill(unsigned ballsCount) override
		{
			m_gumballMachine.AddBalls(ballsCount);
		}

		std::string ToString() const override
		{
			return "waiting for quarter";
		}
	private:
		IGumballMachine & m_gumballMachine;
	};

	class CGumballMachine : private IGumballMachine
	{
	public:
		CGumballMachine(unsigned numBalls)
			: m_soldState(*this)
			, m_soldOutState(*this)
			, m_noQuarterState(*this)
			, m_hasQuarterState(*this)
			, m_state(&m_soldOutState)
			, m_ballsCount(numBalls)
		{
			if (m_ballsCount > 0)
			{
				m_state = &m_noQuarterState;
			}
		}

		void EjectQuarter()
		{
			m_quarters = 0;
			m_state->EjectQuarter();
		}

		void InsertQuarter()
		{
			m_state->InsertQuarter();
		}

		void TurnCrank()
		{
			m_state->TurnCrank();
			m_state->Dispense();
		}

		std::string ToString()const
		{
			auto fmt = boost::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Inventory: %1% gumball%2%
Quarters: %3%
Machine is %4%
)");
			return (fmt % m_ballsCount % (m_ballsCount != 1 ? "s" : "") % m_quarters % m_state->ToString()).str();
		}
	private:
		unsigned GetBallCount() const override
		{
			return m_ballsCount;
		}

		unsigned GetQuartersCount()const override
		{
			return m_quarters;
		}

		void ReleaseBall() override
		{
			if (m_ballsCount != 0)
			{
				std::cout << "A gumball comes rolling out the slot...\n";
				--m_ballsCount;
			}
		}

		void AddQuarter() override
		{
			if (m_quarters > 5)
			{
				++m_quarters;
				std::cout << "You added a quarter" << std::endl;
			}
			else
			{
				std::cout << "Can't add more than 5 quarters" << std::endl;
			}
		}

		void UseQuarter() override
		{
			if (m_quarters != 0)
			{
				--m_quarters;
			}
		}

		void AddBalls(unsigned ballsCount) override
		{
			m_ballsCount += ballsCount;
		}

		void EjectQuarters() override
		{
			std::cout << boost::format(R"(%1% coin%2% returned)") % m_quarters % (m_quarters != 1 ? "s" : "") << std::endl;
			m_quarters = 0;
		}

		void SetSoldOutState() override
		{
			m_state = &m_soldOutState;
		}

		void SetNoQuarterState() override
		{
			m_state = &m_noQuarterState;
		}

		void SetSoldState() override
		{
			m_state = &m_soldState;
		}

		void SetHasQuarterState() override
		{
			m_state = &m_hasQuarterState;
		}
	private:
		unsigned m_ballsCount = 0;
		unsigned m_quarters = 0;
		CSoldState m_soldState;
		CSoldOutState m_soldOutState;
		CNoQuarterState m_noQuarterState;
		CHasQuarterState m_hasQuarterState;
		IState * m_state;
	};
}