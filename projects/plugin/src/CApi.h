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

#ifndef _CAPI_H
	#define _CAPI_H

	#include "MacroConstants.h"
	#include <exception>

	namespace APE
	{

		class Engine;
		struct CSharedInterface;

		// Status definitions for operation and states.
		enum Status {
			STATUS_OK = 0,		// operation completed succesfully
			STATUS_ERROR = 1,	// operation failed, state errornous
			STATUS_WAIT = 2,	// operation not completed yet
			STATUS_SILENT = 3,	// the plugin should not process data
			STATUS_READY = 4,	// ready for any operation
			STATUS_DISABLED = 5,// plugin is disabled
			STATUS_HANDLED = 6, // plugin handled request, host shouldn't do anything.
			STATUS_NOT_IMPLEMENTED = 7 // requested service is not yet implemented.
		};
		/*
			the c-state's function pointers.
		*/
		struct event_ctrlValueChanged {
			float value;
			char text[64];
			char title[64];
			int tag;
			bool unused;
		};

		struct SLine
		{
			int x1, x2, y1, y2;
		};
		
		typedef float(APE_API * ScaleFunc)(float value, float _min, float _max);

		struct CEvent
		{
			// poor attempt at c polymorphism
			enum event_type_t
			{
				ctrlValueChanged = 0



			} event_type;

			union 
			{
				event_ctrlValueChanged * eCtrlValueChanged;

			} event;

		};
		/*
			This programs exposed functions to the c subsystem
		*/
		float		APE_API			getSampleRate(CSharedInterface * iface);
		int			APE_API_VARI	printLine(CSharedInterface * iface, unsigned nColor, const char * fmt, ... );
		int			APE_API			msgBox(CSharedInterface * iface, const char * text, const char * title, int nStyle, int nBlocking);
		enum Status APE_API			setStatus(CSharedInterface * iface, enum Status status);
		int			APE_API			createKnob(CSharedInterface * iface, const char * name, float * extVal, int type);
		long long	APE_API			timerGet(CSharedInterface * iface);
		double		APE_API			timerDiff(CSharedInterface * iface, long long time);
		void *		APE_API			alloc(CSharedInterface * iface, size_t size);
		void		APE_API			free(CSharedInterface * iface, void * ptr);
		int			APE_API			createKnobEx(CSharedInterface * iface, const char * name, float * extVal, char * values, char * unit);
		void		APE_API			setInitialDelay(CSharedInterface * iface, int samples);
		int			APE_API_VARI	createLabel(CSharedInterface * iface, const char * name, const char * fmt, ...);
		int			APE_API			getNumInputs(CSharedInterface * iface);
		int			APE_API			getNumOutputs(CSharedInterface * iface);
		int			APE_API			createMeter(CSharedInterface * iface, const char * name, float * extVal);
		int			APE_API			createToggle(CSharedInterface * iface, const char * name, float * extVal);
		double		APE_API			getBPM(CSharedInterface * iface);
		float		APE_API			getCtrlValue(CSharedInterface * iface, int iD);
		void		APE_API			setCtrlValue(CSharedInterface * iface, int iD, float value);
		int			APE_API			createPlot(CSharedInterface * iface, const char * name, const float * const vals, const unsigned int numVals);
		int			APE_API			createRangeKnob(CSharedInterface * iface, const char * name, const char * unit, float * extVal, ScaleFunc scaleCB, float min, float max);
		int			APE_API			createLinePlot(CSharedInterface * iface, const char * name, const SLine * const vals, const unsigned int numVals);
	};
#endif