/*
 * OpDiLib, an Open Multiprocessing Differentiation Library
 *
 * Copyright (C) 2020-2022 Chair for Scientific Computing (SciComp), TU Kaiserslautern
 * Homepage: http://www.scicomp.uni-kl.de
 * Contact:  Prof. Nicolas R. Gauger (opdi@scicomp.uni-kl.de)
 *
 * Lead developer: Johannes Blühdorn (SciComp, TU Kaiserslautern)
 *
 * This file is part of OpDiLib (http://www.scicomp.uni-kl.de/software/opdi).
 *
 * OpDiLib is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * OpDiLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU
 * General Public License along with OpDiLib.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * For other licensing options please contact us.
 *
 */

#include "opdi.hpp"

// global variables

opdi::ToolInterface* opdi::tool = nullptr;
opdi::LogicInterface* opdi::logic = nullptr;
opdi::BackendInterface* opdi::backend = nullptr;

omp_lock_t opdi::Output::lock;
bool opdi::TapedOutput::active = OPDI_OMP_LOGIC_INSTRUMENT;

// include logic source

#include "opdi/logic/omp/ompLogic.cpp"

// include appropriate backend source

#ifndef OPDI_BACKEND
  #error Please include at least one backend.
#elif OPDI_BACKEND == OPDI_MACRO_BACKEND
  #include "opdi/backend/macro/macroBackend.cpp"
#elif OPDI_BACKEND == OPDI_OMPT_BACKEND
  #include "opdi/backend/ompt/omptBackend.cpp"
#else
  #error Unknown backend.
#endif
