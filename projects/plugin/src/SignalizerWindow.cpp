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
 
	file:SignalizerWindow.cpp
	
		Implementation of the API functions.

*************************************************************************************/

#include <cpl/Common.h>
#include "SignalizerWindow.h"
#include "Common/SignalizerDesign.h"
#include "Engine.h"

namespace ape 
{


	SignalizerWindow::SignalizerWindow(OscilloscopeData& oData)
		: juce::Component("Oscilloscope")
		, data(oData)
		, scope(data.getBehaviour(), data.getName(), data.getStream(), &data.getContent())
	{
		setVisible(true);

		editor = data.getContent().createEditor();
		editor->setSize(600, 200);
		editor->setVisible(true);
		editor->setCentrePosition(1000, 1000);
		editor->setOpaque(true);
		editor->addToDesktop(juce::ComponentPeer::StyleFlags::windowIsResizable | juce::ComponentPeer::StyleFlags::windowHasCloseButton | juce::ComponentPeer::StyleFlags::windowHasTitleBar);

		addAndMakeVisible(&scope);
		context.setMultisamplingEnabled(true);

		juce::OpenGLPixelFormat format;
		format.multisamplingLevel = 16;
		context.setPixelFormat(format);

		scope.attachToOpenGL(context);
	}

	void SignalizerWindow::resized()
	{
		scope.setSize(getWidth(), getHeight());
	}

	SignalizerWindow::~SignalizerWindow()
	{
		scope.detachFromOpenGL(context);
	}

}