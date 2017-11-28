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

	file:CSerializer.h
		Provides static methods for serializing objects of type 'Engine' into a memoryblock.

*************************************************************************************/

#ifndef _CSERIALIZER_H
	#define _CSERIALIZER_H


	#include <cpl/MacroConstants.h>
	#include "Engine.h"
	#include "UIController.h"
	#include "CApi.h"
	#include "Common.h"
	#include "CConsole.h"
	#include <cpl/CThread.h>
	#include "CCodeEditor.h"

	namespace ape
	{
		/*
			It is of pretty high importance to standardize the layout of this 
			struct os-, architechture- and compiler independent (for obvious reasons)
		*/
		struct SerializedEngine
		{
			typedef uint32_t SeIntType;

			struct ControlValue
			{
				float value;
				SeIntType tag;

			};

			SeIntType
				structSize, // sizeof this struct
				version, // the version that created this struct ( typically _VERSION_INT)
				size, // the total size of this memory block
				fileNameOffset, // the offset from which the filename is stored as a char
				numValues, // the number of ControlValue values that exists in this block
				valueOffset, // the offset from which the array of ControlValues resides in this block
				editorOpened, // whether the editor was opened
				hasAProject, // whether the state has a project
				isActivated; // whether the state was activated


			const char * getFileNameConst() const
			{
				if (fileNameOffset > size)
					return nullptr;
				return reinterpret_cast<const char*>(reinterpret_cast<const char*>(this) + fileNameOffset);
			}
			const ControlValue * getValuesConst() const
			{
				if (valueOffset > size)
					return nullptr;
				return reinterpret_cast<const ControlValue*>(reinterpret_cast<const char*>(this) + valueOffset);
			}
			char * getFileName()
			{
				if (fileNameOffset > size)
					return nullptr;
				return reinterpret_cast<char*>(reinterpret_cast<char*>(this) + fileNameOffset);
			}
			ControlValue * getValues()
			{
				if (valueOffset > size)
					return nullptr;
				return reinterpret_cast<ControlValue*>(reinterpret_cast<char*>(this) + valueOffset);
			}
		};

		class CSerializer
		{
		public:

			static bool serialize(Engine * engine, juce::MemoryBlock & destination)
			{

				std::string fileName = engine->getController().externEditor->getDocumentPath();
				bool hasAProject = fileName.size() > 2 ? true : false;
				Status state = engine->getCState() ? engine->getCState()->getState() : STATUS_DISABLED;

				// we basically quantize all engine states to running or not running
				// anything in between is error states or intermediate states.
				bool isActivated(false);

				switch (state)
				{
				case Status::STATUS_ERROR:
				case Status::STATUS_DISABLED:
					isActivated = false;
					break;
				case Status::STATUS_OK:
				case Status::STATUS_READY:
					isActivated = true;
					break;
				default:
					isActivated = false;
				}

				auto listSize = isActivated ? engine->getCState()->getCtrlManager().getControls().size() : 0;

				// needed size in bytes
				std::size_t neededSize =
					sizeof(SerializedEngine) +
					listSize * sizeof(SerializedEngine::ControlValue) +
					fileName.size() + 1;

				SerializedEngine * se = reinterpret_cast<SerializedEngine*>(malloc(neededSize));
				::memset(se, 0, neededSize);
				se->size = neededSize;
				se->structSize = sizeof(SerializedEngine);
				se->version = 9;
				se->editorOpened = engine->getController().externEditor->isOpen();
				se->isActivated = isActivated;
				se->valueOffset = sizeof(SerializedEngine);
				se->fileNameOffset = se->valueOffset + listSize * sizeof(SerializedEngine::ControlValue);
				se->hasAProject = hasAProject;
				se->numValues = listSize;
				::memcpy(se->getFileName(), fileName.c_str(), fileName.size() + 1);

				if (isActivated)
				{
					SerializedEngine::ControlValue * values = se->getValues();
					int i = 0;
					for (auto ctrl : engine->getCState()->getCtrlManager().getControls())
					{
						values[i].tag = ctrl->bGetTag();
						values[i].value = ctrl->bGetValue();

						i++;
					}
				}


				destination.ensureSize(neededSize);
				destination.copyFrom(se, 0, neededSize);
				::free(se);
				return true;
			}

			static bool restore(Engine * engine, const void * block, unsigned size)
			{
				using namespace cpl;
				const SerializedEngine * se = reinterpret_cast<const SerializedEngine *> (block);
				// some basic checks
				if (
					!se // nullpointer check
					|| size < sizeof(SerializedEngine) // whether the actual size is smaller than the 
					|| se->size != size // whether the size actual size matches reported size
					)
				{
					engine->getController().console().printLine(CColours::red,
						"[Serializer] : Invalid memory block recieved from host (%d, %d, %d, %d)!", 
						se, size, sizeof(SerializedEngine), se->size);
					return false;
				}
				else if (se->version != 9)
				{
					engine->getController().console().printLine(CColours::red,
						"[Serializer] : Warning: Different versions detected!");

					auto answer = Misc::MsgBox
						(
						"Warning: You're trying to restore an instance from a different version (0) of this plugin, continue?", 
							cpl::programInfo.name + " warning",
						Misc::MsgStyle::sYesNoCancel | Misc::MsgIcon::iWarning
						);
					if (answer != Misc::MsgButton::bYes)
						return false;
				}
				// first we open the editor to ensure it's initialized
				if(!engine->getController().externEditor->openEditor(se->editorOpened))
				{
					engine->getController().console().printLine(CColours::red,
						"[Serializer] : Error opening editor!");
					return false;
				}
				if (engine->getController().externEditor->checkAutoSave())
				{
					engine->getController().console().printLine(CColours::red,
						"[Serializer] : Autosave was restored, reopen the project to perform normal serialization.");
					return false;
				}
				// then, we set it to the file from last session
				if(!engine->getController().externEditor->openFile(se->getFileNameConst()))
				{
					engine->getController().console().printLine(CColours::red,
						"[Serializer] : Error opening session file (%s)!", se->getFileNameConst());
					return false;
				}
				// check if the project was running:
				if (se->isActivated)
				{
					// try to compile the project
					auto plugin = engine->getController().createPlugin().get();

					// check if success
					if(!plugin)
					{
						engine->getController().console().printLine(CColours::red,
							"[Serializer] : Error compiling session file (%s)!", se->getFileNameConst());
						return false;
					}

					engine->exchangePlugin(std::move(plugin));

					// project is now compiled, lets try to activate it
					if (!engine->activatePlugin())
					{
						engine->getController().console().printLine(CColours::red,
							"[Serializer] : Error activating project (%s)!", engine->getController().projectName.c_str());
						return false;

					}
					// project is up and running! now we just need to reset parameters
					// get values
					const SerializedEngine::ControlValue * values = se->getValuesConst();
					// get control manager
					CPluginCtrlManager & ctrlManager = engine->getCState()->getCtrlManager();
				
					for (unsigned i = 0; i < se->numValues; i++)
					{
						// get instance control from tag
						auto control = ctrlManager.getControl(values[i].tag);
						// if it exists, set the value of it
						
						if (control)
							control->bSetValue(values[i].value);
						else
						{
							engine->getController().console().printLine(CColours::red,
								"[Serializer] : Error restoring values to controls: No control found for tag %d!",
								values[i].tag);
							return false;

						}
					}
					// all controls restored - now we update display, and were set!
					ctrlManager.callListeners();
				}

				return true;

			}




		};
	};
#endif