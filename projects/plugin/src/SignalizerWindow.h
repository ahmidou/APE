/*************************************************************************************
 
	 Audio Programming Environment - Audio Plugin - v. 0.3.0.
	 
	 Copyright (C) 2014 Janus Lynggaard Thorborg [LightBridge Studios]
	 
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

	file:CApi.h

		The API the C program can utilize, definitions for exported functions/symbols.

*************************************************************************************/

#ifndef SIGNALIZERWINDOW_H
	#define SIGNALIZERWINDOW_H

	#include "SignalizerConfiguration.h"
	#include "Oscilloscope/Oscilloscope.h"

	namespace ape
	{
		class Engine;

		struct OscilloscopeData : private Signalizer::ParameterSet::AutomatedProcessor
		{


			OscilloscopeData()
				: stream(4, true)
				, view(stream, *this)
				, content(0, false, view)
			{
				behaviour.hideWidgetsOnMouseExit = true;
				behaviour.stopProcessingOnSuspend = true;
			}

		public:

			Signalizer::AudioStream& getStream() noexcept { return stream; }
			Signalizer::SharedBehaviour& getBehaviour() noexcept { return behaviour; }
			Signalizer::OscilloscopeContent& getContent() noexcept { return content; }
			const std::string& getName() const noexcept { return name; }

		private:

			virtual void automatedTransmitChangeMessage(int parameter, Signalizer::ParameterSet::FrameworkType value) override {}
			virtual void automatedBeginChangeGesture(int parameter) override {}
			virtual void automatedEndChangeGesture(int parameter) override {}

			const std::string name = "scope";
			Signalizer::SharedBehaviour behaviour;
			Signalizer::AudioStream stream;
			Signalizer::SystemView view;
			Signalizer::OscilloscopeContent content;

		};

		class SignalizerWindow : public juce::DocumentWindow
		{
		public:

			SignalizerWindow(OscilloscopeData& engine);
			~SignalizerWindow();

		private:
			OscilloscopeData& data;
			Signalizer::Oscilloscope scope;
			std::unique_ptr<Signalizer::StateEditor> editor;
			juce::OpenGLContext context;
		};
	};

#endif