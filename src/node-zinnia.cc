/**
 * @file
 * requireで読み込まれるアドオンのエントリーポイントとクラス.
 * @author hankei6km
 */
#include <node.h>
#include <v8.h>
#include <zinnia.h>
#include "node-zinnia.h"
#include "node-zinnia-character.h"
#include "node-zinnia-recognizer.h"
#include "node-zinnia-result.h"

using namespace v8;

Handle<Value> GetCharacter(const Arguments& args) {
  HandleScope scope;
  return scope.Close(Character::NewInstance(args));
}
Handle<Value> GetRecognizer(const Arguments& args) {
  HandleScope scope;
  return scope.Close(Recognizer::NewInstance(args));
}
Handle<Value> GetResult(const Arguments& args) {
  HandleScope scope;
  return scope.Close(Result::NewInstance(args));
}

void InitAll(Handle<Object> exports) {
  Character::Init();
  Recognizer::Init();
  Result::Init();

  exports->Set(String::NewSymbol("Character"),
      FunctionTemplate::New(GetCharacter)->GetFunction());
  exports->Set(String::NewSymbol("Recognizer"),
      FunctionTemplate::New(GetRecognizer)->GetFunction());
  exports->Set(String::NewSymbol("Result"),
      FunctionTemplate::New(GetResult)->GetFunction());
}

NODE_MODULE(zinnia, InitAll)
