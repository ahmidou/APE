/*************************************************************************************
 
	 Audio Programming Environment - Audio Plugin - v. 0.3.0.
	 
	 Copyright (C) 2018 Janus Lynggaard Thorborg [LightBridge Studios]
	 
	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.
	 
	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.
	 
	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.
	 
	 See \licenses\ for additional details on licenses associated with this program.
 
 **************************************************************************************

	file:PlayStateButton.h
	
		Main play/pause animation button for triggering loading effects into audio
		stream.

*************************************************************************************/

#ifndef APE_PLAYSTATEBUTTON_H
#define APE_PLAYSTATEBUTTON_H

#include <cpl/gui/controls/ValueControl.h>
#include <cpl/gui/controls/ControlBase.h>
#include <chrono>

namespace ape
{
	class PlayStateButton
		: public juce::Button
		, public cpl::ValueControl<cpl::ValueEntityBase, cpl::CompleteValue<cpl::LinearRange<cpl::ValueT>, cpl::BasicFormatter<cpl::ValueT>>>
		, private juce::Timer
	{

		typedef cpl::ValueControl<cpl::ValueEntityBase, cpl::CompleteValue<cpl::LinearRange<cpl::ValueT>, cpl::BasicFormatter<cpl::ValueT>>> Base;

	public:

		PlayStateButton(cpl::ValueEntityBase& compilationState, cpl::ValueEntityBase& activationState);
		~PlayStateButton();

		void clicked() override;
		std::string bGetTitle() const override;

		void timerCallback() override;

	private:
		void onValueObjectChange(ValueEntityListener * sender, cpl::ValueEntityBase * object) override;
		void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

		std::chrono::steady_clock::time_point timeAtCompilationStart;
		cpl::ValueEntityBase& activationState;
	};
}
#endif