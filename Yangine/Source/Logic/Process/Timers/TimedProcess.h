#pragma once
/** \file TimedProcess.h */
/** Timed process description */

#include "..\IProcess.h"

//! \namespace yang Contains all Yangine code
namespace yang
{
	/** \class TimedProcess */
	/** Executes Succeed callback after some amount of seconds */
	template<class Function, class... DataArgs>
	class TimedProcess
		: public IProcess
	{
	public:
		// --------------------------------------------------------------------- //
		// Public Member Variables
		// --------------------------------------------------------------------- //


		// --------------------------------------------------------------------- //
		// Public Member Functions
		// --------------------------------------------------------------------- //

		/// Constructor
		/// \param pOwner - actor that owns this process
		/// \param executionTime - number of seconds to execute this process
		TimedProcess(Actor* pOwner, float executionTime, Function&& f, DataArgs&&... data);

		/** Default Destructor */
		virtual ~TimedProcess() = default;

		/// Updates the process by deltaSeconds.
		/// \param deltaSeconds - amount of seconds passed since last frame
		virtual void Update(float deltaSeconds) override final;

	private:
		// --------------------------------------------------------------------- //
		// Private Member Variables
		// --------------------------------------------------------------------- //
		float m_executionTime;  ///< amount of seconds to execute process
		std::tuple<DataArgs...> m_data;
		Function m_executionFunction;

		// --------------------------------------------------------------------- //
		// Private Member Functions
		// --------------------------------------------------------------------- //


	public:
		// --------------------------------------------------------------------- //
		// Accessors & Mutators
		// --------------------------------------------------------------------- //


	};
	template<class Function, class... DataArgs>
	inline TimedProcess<Function, DataArgs...>::TimedProcess(Actor* pOwner, float executionTime, Function&& f, DataArgs&&... data)
		:IProcess(pOwner)
		,m_executionTime(executionTime)
		,m_executionFunction(std::move(f))
		,m_data(std::forward_as_tuple(data...))
	{
	}
	template<class Function, class... DataArgs>
	inline void TimedProcess<Function, DataArgs...>::Update(float deltaSeconds)
	{
		std::apply(m_executionFunction, std::tuple_cat(m_data, std::make_tuple(deltaSeconds)));
		m_executionTime -= deltaSeconds;
		if (m_executionTime <= 0)
		{
			Succeed();
		}
	}
}