/**
 * @file
 * requireで読み込まれるアドオンのエントリーポイントとクラス.
 * @author hankei6km
 */
#include <node.h>
#include <v8.h>
#include <zinnia.h>
#include "node-zinnia.h"
#include "node-zinnia-recognizer.h"

using namespace v8;

Handle<Value> GetRecognizer(const Arguments& args) {
  HandleScope scope;
  return scope.Close(Recognizer::NewInstance(args));
}

void InitAll(Handle<Object> exports) {
  Recognizer::Init();

  exports->Set(String::NewSymbol("Recognizer"),
      FunctionTemplate::New(GetRecognizer)->GetFunction());
}

NODE_MODULE(zinnia, InitAll)
