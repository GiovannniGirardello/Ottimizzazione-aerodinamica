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

#pragma once

#include "../logicInterface.hpp"

namespace opdi {

  struct SyncRegionOmpLogic : public virtual LogicInterface {
    public:

      using LogicInterface::ScopeEndpoint;
      using LogicInterface::SyncRegionKind;

      struct Data {
        public:
          SyncRegionKind kind;
          ScopeEndpoint endpoint;
      };

    private:

      static void reverseFunc(void* dataPtr);
      static void deleteFunc(void* dataPtr);

      void internalPushHandle(Data* data);

    public:

      virtual void onSyncRegion(SyncRegionKind kind, ScopeEndpoint endpoint);
      virtual void addReverseBarrier();
  };
}
