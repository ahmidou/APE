/*************************************************************************************
 
	 Audio Programming Environment - Audio Plugin - v. 0.3.0.
	 
	 Copyright (C) 2017 Janus Lynggaard Thorborg [LightBridge Studios]
	 
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

	file:ProtoCompiler.h
	
		Defines a prototype (well, abstract class) of a compiler to be used
		together with CompilerBindings.h and CppCompilerInterface.cpp,
		providing automatic bindings with ape.

*************************************************************************************/

#ifndef APE_PROTOCOMPILER_H
	#define APE_PROTOCOMPILER_H

	#include "Ape.h"
	#include "Project.h"
	#include "Events.h"
	#include <utility>
	#include <string>

	namespace ape
	{
		class ProtoCompiler
		{
		public:

			// changes current project
			void setProject(Project * p) { project = p; }
			Project * getProject() noexcept { return project; }
			// prints to error function
			void print(const char * s)
			{
				if(errorFunc)
					errorFunc(opaque, s);
			}

			void print(const std::string& s)
			{
				print(s.c_str());
			}

			void setErrorFunc(void * op, ErrorFunc e)
			{
				opaque = op;
				errorFunc = e;
			}

			// wrappers for the compiler api
			virtual Status processReplacing(float ** in, float ** out, int frames) = 0;
			virtual Status compileProject() = 0;
			virtual Status releaseProject() = 0;
			virtual Status initProject() = 0;
			virtual Status activateProject() = 0;
			virtual Status disableProject(bool didMisbehave = false) = 0;

			virtual Status onEvent(Event * e)
			{
				return STATUS_NOT_IMPLEMENTED;
			}

			virtual ~ProtoCompiler() {}

			std::pair<void *, ErrorFunc> getErrorFuncDetails() const noexcept { return { opaque, errorFunc }; }

		private:
			// opaque pointer to be used when error printing
			void * opaque = nullptr;
			// error function
			ErrorFunc errorFunc = nullptr;
			// the project our instance is associated with
			Project * project = nullptr;

		};
	}
#endif