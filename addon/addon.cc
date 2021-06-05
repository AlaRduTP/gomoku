#include <node.h>
#include "agent.h"

namespace addon {

using namespace agent;

using v8::Local;
using v8::Object;

NODE_MODULE_INIT() {
	Agent::Init(exports);
}

}