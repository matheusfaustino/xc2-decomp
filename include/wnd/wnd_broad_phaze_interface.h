#pragma once

#include "mm_mtl/intrusive_list.h"
#include "mm_mtl/reference_object.h"

namespace wnd {

struct Query;
class Dispatcher;
class WindObject;
class WindProbe;

// Interface of the broad phase used by WindWorld; the slot order comes from the vtable of BroadPhazeOctree (0x71011b9ba8).
class BroadPhazeInterface : public mm::mtl::ReferenceObject {
public:
  virtual ~BroadPhazeInterface() = default;
  virtual bool createProxy(WindObject &wind);
  virtual void destroyProxy(WindObject &wind);
  virtual bool addWindObject(WindObject &wind);
  virtual void removeWindObject(WindObject &wind);
  virtual bool calculateOverlapping(Dispatcher &dispatcher, mm::mtl::IntrusiveList<WindProbe> &probes);
  virtual bool query(Query *query);
};

} // namespace wnd
