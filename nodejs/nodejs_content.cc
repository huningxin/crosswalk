#include "xwalk/nodejs/nodejs_content.h"
#include "third_party/node/src/node_webkit.h"
#include "base/command_line.h"
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/native_library.h"
#include "base/path_service.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "base/threading/thread_restrictions.h"
#include "third_party/WebKit/public/web/WebScopedMicrotaskSuppression.h"
#include "third_party/WebKit/Source/platform/ScriptForbiddenScope.h"

CallTickCallbackFn g_call_tick_callback_fn = nullptr;
SetupNWNodeFn g_setup_nwnode_fn = nullptr;
IsNodeInitializedFn g_is_node_initialized_fn = nullptr;
SetNWTickCallbackFn g_set_nw_tick_callback_fn = nullptr;
StartNWInstanceFn g_start_nw_instance_fn = nullptr;
GetNodeContextFn g_get_node_context_fn = nullptr;
SetNodeContextFn g_set_node_context_fn = nullptr;
GetNodeEnvFn g_get_node_env_fn = nullptr;
GetCurrentEnvironmentFn g_get_current_env_fn = nullptr;
EmitExitFn g_emit_exit_fn = nullptr;
RunAtExitFn g_run_at_exit_fn = nullptr;
NodeStartFn g_node_start_fn = nullptr;
UVRunFn g_uv_run_fn = nullptr;
SetUVRunFn g_set_uv_run_fn = nullptr;


namespace xwalk {
namespace nodejs {

v8::Handle<v8::Value> CallNWTickCallback(void* env, const v8::Handle<v8::Value> ret) {
  blink::WebScopedMicrotaskSuppression suppression;
  g_call_tick_callback_fn(env);
  return Undefined(v8::Isolate::GetCurrent());
}

int nw_uv_run(void* loop, int mode) {
  blink::WebScopedMicrotaskSuppression suppression;
  return g_uv_run_fn(loop, mode);
}

void DidCreateScriptContext(blink::WebLocalFrame* frame, v8::Handle<v8::Context> context) {
  if (g_is_node_initialized_fn == nullptr)
  	LoadNodeSymbols();

  if (!g_is_node_initialized_fn()) {
    g_setup_nwnode_fn(0, nullptr);
    g_set_uv_run_fn(nw_uv_run);
  }

  int argc = 1;
  char argv0[] = "node";
  char* argv[3];
  argv[0] = argv0;
  argv[1] = argv[2] = nullptr;

  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  blink::WebScopedMicrotaskSuppression suppression;

  g_set_nw_tick_callback_fn(&CallNWTickCallback);
  v8::Local<v8::Context> dom_context = context;
  dom_context->SetSecurityToken(v8::String::NewFromUtf8(isolate, "nw-token"));
  dom_context->Enter();

  g_start_nw_instance_fn(argc, argv, dom_context);

  {
  	//FIXME
    std::string root_path;
    base::ReplaceChars(root_path, "'", "\\'", &root_path);
    v8::Local<v8::Script> script = v8::Script::Compile(v8::String::NewFromUtf8(isolate,
     ("global.__dirname = '" + root_path + "';").c_str()));
    script->Run();
  }

  dom_context->Exit();
}

void WillReleaseScriptContext(blink::WebLocalFrame* frame, v8::Handle<v8::Context> context) {
  blink::WebScopedMicrotaskSuppression suppression;
  blink::ScriptForbiddenScope::AllowUserAgentScript script;
  void* env = g_get_current_env_fn(context);
  if (g_is_node_initialized_fn()) {
    g_emit_exit_fn(env);
    g_run_at_exit_fn(env);
  }
}

void LoadNodeSymbols() {
  base::NativeLibraryLoadError error;
  base::FilePath node_dll_path = base::FilePath::FromUTF16Unsafe(base::GetNativeLibraryName(base::UTF8ToUTF16("node")));
  base::NativeLibrary node_dll = base::LoadNativeLibrary(node_dll_path, &error);
  if(!node_dll)
    LOG_IF(FATAL, true) << "Failed to load node library (error: " << error.ToString() << ")";
  else {
    g_node_start_fn = (NodeStartFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_node_start");
    g_uv_run_fn = (UVRunFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_uv_run");
    g_set_uv_run_fn = (SetUVRunFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_set_uv_run");
    g_call_tick_callback_fn = (CallTickCallbackFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_call_tick_callback");
    g_setup_nwnode_fn = (SetupNWNodeFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_setup_nwnode");
    g_is_node_initialized_fn = (IsNodeInitializedFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_is_node_initialized");
    g_set_nw_tick_callback_fn = (SetNWTickCallbackFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_set_nw_tick_callback");
    g_start_nw_instance_fn = (StartNWInstanceFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_start_nw_instance");
    g_get_node_context_fn = (GetNodeContextFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_get_node_context");
    g_set_node_context_fn = (SetNodeContextFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_set_node_context");
    g_get_node_env_fn = (GetNodeEnvFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_get_node_env");
    g_get_current_env_fn = (GetCurrentEnvironmentFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_get_current_env");
    g_emit_exit_fn = (EmitExitFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_emit_exit");
    g_run_at_exit_fn = (RunAtExitFn)base::GetFunctionPointerFromNativeLibrary(node_dll, "g_run_at_exit");
  }
}

}
}