/**
 * @file
 * Recognizerクラス.
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

Recognizer::Recognizer() {};
Recognizer::~Recognizer() {
  delete recognizer_;
};

Persistent<Function> Recognizer::constructor;
Persistent<Function> rc = Persistent<Function>::New(FunctionTemplate::New(Result::NewInstance)->GetFunction());

void Recognizer::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Recognizer"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("open"),
      FunctionTemplate::New(Open)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("classify"),
      FunctionTemplate::New(Classify)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Recognizer::New(const Arguments& args) {
  HandleScope scope;

  Recognizer* obj = new Recognizer();
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

Handle<Value> Recognizer::Open(const Arguments& args) {
  HandleScope scope;

  bool ret = false;
  if(args.Length()==1 && args[0]->IsString()){
    Recognizer* obj = ObjectWrap::Unwrap<Recognizer>(args.This());

    String::Utf8Value v8str(args[0]);
    ret = obj->recognizer_->open(*v8str);
  }else{
    return ThrowException(
        Exception::Error(String::New("open argument require filename.")));
  }

  return scope.Close(Boolean::New(ret));
}

Handle<Value> Recognizer::Classify(const Arguments& args) {
  HandleScope scope;

  Local<Object> instance;
  if(args.Length()==2 && args[0]->IsObject() && args[1]->IsNumber()){
    Recognizer* obj = ObjectWrap::Unwrap<Recognizer>(args.This());

    Character* c = node::ObjectWrap::Unwrap<Character>(
      args[0]->ToObject());

    zinnia::Result *result
      = obj->recognizer_->classify(*(c->character()), args[1]->NumberValue());

    const unsigned argc = 1;
    Handle<Value> argv[argc] = { External::New(result) };
    instance = rc->NewInstance(argc, argv);
  }else{
    return ThrowException(
        Exception::Error(String::New("classify argument require character, nbest.")));
  }

  return scope.Close(instance);
}
