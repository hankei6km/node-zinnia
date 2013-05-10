/**
 * @file
 * requireで読み込まれるアドオンのエントリーポイントとクラス.
 * @author hankei6km
 */
#include <node.h>
#include <v8.h>
#include <zinnia.h>
#include "node-zinnia.h"

using namespace v8;

Recognizer::Recognizer() {};
Recognizer::~Recognizer() {};

Persistent<Function> Recognizer::constructor;

void Recognizer::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Recognizer"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("open"),
      FunctionTemplate::New(RecognizerOpen)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Recognizer::New(const Arguments& args) {
  HandleScope scope;

  Recognizer* obj = new Recognizer();
  //obj->counter_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
  obj->recognizer_ = zinnia::Recognizer::create();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> Recognizer::NewInstance(const Arguments& args) {
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Handle<Value> Recognizer::RecognizerOpen(const Arguments& args) {
  HandleScope scope;


  return scope.Close(Undefined());
}

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
