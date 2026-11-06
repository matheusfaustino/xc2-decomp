#pragma once
#include <stddef.h>

#include "gf/object/gf_obj_manager.h"
#include "mm_math/mm_vec3.h"
#include "mm_mtl/fixed_list.h"
#include "mm_mtl/singleton.h"
#include "types.h"

namespace gf {
struct GF_OBJ_HANDLE;
} // namespace gf

namespace fw {
struct UpdateInfo;
}

namespace ai {

// Layout evidence: AiData::AiData (0x71000022d8) stores the vtable at 0x00, the handle at 0x08, the id at 0x10, the
// sleep flag at 0x14, the frame count at 0x18 and clears the sixteen params that follow; AiManager::regist allocates
// 0x60 bytes, 0x10 aligned, and AiData::getParam reads at 0x1c.
class AiData {
public:
  AiData();
  virtual ~AiData();

  void init();
  void destroy();
  void update(const fw::UpdateInfo &info);
  gf::GF_OBJ_HANDLE *getHandle() const;
  void setHandle(gf::GF_OBJ_HANDLE *handle);
  gf::GF_OBJ_HANDLE *getHandleTarget() const;
  int getId() const;
  void setId(int id);
  bool isSleep() const;
  void setSleep(bool isSleep);
  bool isValid() const;
  int getParam(int index) const;
  void setParam(int index, int value);
  void setFrameCount(f32 frameCount);
  bool isFrameCountEnd() const;
  mm::Vec3 getPosition() const;
  mm::Vec3 getPosition(gf::GF_OBJ_HANDLE *handle) const;
  mm::Vec3 getPositionTarget() const;
  f32 getRadius() const;
  f32 getRadius(gf::GF_OBJ_HANDLE *handle) const;
  f32 getRadiusTarget() const;
  int getScoutStartHp() const;

  gf::GF_OBJ_HANDLE *m_handle; // Named by getHandle and setHandle; INVALID_HANDLE when unused.
  int m_id;                    // Named by getId and setId.
  bool m_sleep;                // Named by isSleep and setSleep.
  f32 m_frameCount;            // Named by setFrameCount and isFrameCountEnd.
  int m_param[16];             // Named by getParam and setParam.
};

static_assert(offsetof(AiData, m_handle) == 0x08, "");
static_assert(offsetof(AiData, m_id) == 0x10, "");
static_assert(offsetof(AiData, m_sleep) == 0x14, "");
static_assert(offsetof(AiData, m_frameCount) == 0x18, "");
static_assert(offsetof(AiData, m_param) == 0x1c, "");
static_assert(sizeof(AiData) == 0x60, "");

// Layout evidence: AiScriptManager::start (0x710000388c) walks the 32 entries that begin at 0x08; an entry holds its
// script name at 0x00, its reference count at 0x08 and its state at 0x0c. AiManager::_clear calls into the manager to
// end the script of each data it drops.
class AiScriptManager : public mm::mtl::Singleton<AiScriptManager> {
public:
  // Inferred roles and names of the script states: original enumerator names unknown.
  enum unknown001 {
    UNKNOWN001_UNKNOWN_0 = 0, // _end sets it; _searchData and _searchEmpty treat it as a free slot.
    UNKNOWN001_UNKNOWN_1 = 1, // _update calls _start, which requests the script load and sets state 2.
    UNKNOWN001_UNKNOWN_2 = 2, // _update calls _load, which starts the script and sets state 3.
    UNKNOWN001_UNKNOWN_3 = 3, // _load sets it once the script runs; reload accepts it.
    UNKNOWN001_UNKNOWN_4 = 4, // _update calls _end, which destroys the script and clears the slot.
    UNKNOWN001_UNKNOWN_5 = 5, // reload requests it; _update calls _reload and returns to state 1.
  };

  struct ScriptData {
    const char *unknown00; // Inferred role: script name; original member name unknown.
    int unknown08;         // Inferred role: reference count; original member name unknown.
    int unknown0c;         // Inferred role: state; original member name unknown.
  };

  static const u32 SCRIPT_COUNT = 32;

  static void init();
  static void destroy();
  static void update(const fw::UpdateInfo &info);
  // The game defines start and end out of line; not reconstructed yet.
  static bool start(const char *name);
  static bool end(const char *name);
  static bool reload(const char *name);

private:
  void _clear();
  void _update();
  void _start(int index);
  void _load(int index);
  void _reload(int index);
  void _end(int index);
  int _getScriptType(int index);
  int _searchData(const char *name);
  int _searchEmpty();

public:
  ScriptData unknown08[SCRIPT_COUNT] = {}; // Inferred role: script slots; original member name unknown.
};

static_assert(offsetof(AiScriptManager, unknown08) == 0x08, "");
static_assert(sizeof(AiScriptManager) == 0x208, "");

// Layout evidence: AiScriptDataManager::init (0x7100003080) constructs the singleton, storing the vtable at 0x00 and
// initialising the FixedList at 0x08; AiScriptDataManager::_init pushes the "Script" column of bdat table 0 and
// getScriptName walks that list.
class AiScriptDataManager : public mm::mtl::Singleton<AiScriptDataManager> {
public:
  struct ScriptData {
    int unknown00;         // Inferred role: bdat row id; original member name unknown.
    const char *unknown08; // Inferred role: script name; original member name unknown.
  };

  static const u32 SCRIPT_COUNT = 0x200;

  static void init();
  static void destroy();
  static void update(const fw::UpdateInfo &info);
  static const char *getScriptName(int id);

private:
  void _init();

public:
  mm::mtl::FixedList<ScriptData, SCRIPT_COUNT> unknown08; // Inferred role: script list; original member name unknown.
};

static_assert(sizeof(AiScriptDataManager::ScriptData) == 0x10, "");
static_assert(offsetof(AiScriptDataManager, unknown08) == 0x08, "");
static_assert(sizeof(AiScriptDataManager) == 0x3430, "");

// Layout evidence: AiManager::init (0x71000028f0) adds the IGfObjListener subobject at 0x08 to the object manager, so
// the Singleton base holds the vtable at 0x00 and the listener at 0x08; regist/update/_clear walk the
// FixedList<AiData *, 128> at 0x10.
class AiManager : public mm::mtl::Singleton<AiManager>, public gf::IGfObjListener {
public:
  static const u32 DATA_COUNT = 128;

  void init();
  void destroy();
  void update(const fw::UpdateInfo &info);
  void regist(gf::GF_OBJ_HANDLE *handle, int id);
  void unregist(gf::GF_OBJ_HANDLE *handle);
  AiData *getDataFromHandle(gf::GF_OBJ_HANDLE *handle);
  // The game defines it out of line; not reconstructed yet.
  AiData *getDataFromId(int id, int index);
  int getNumFromId(int id);
  void listenerGfObj(OBJEVT event, gf::GF_OBJ_HANDLE *handle, u32 objectType) override;

private:
  void _clear();

public:
  mm::mtl::FixedList<AiData *, DATA_COUNT> unknown10; // Inferred role: registered datas; original member name unknown.
};

static_assert(offsetof(AiManager, unknown10) == 0x10, "");
static_assert(sizeof(AiManager) == 0x8b0, "");

} // namespace ai
