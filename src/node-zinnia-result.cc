/**
 * @file
 * Result クラス.
 * @author hankei6km
 */
#include <node.h>
#include <v8.h>
#include <zinnia.h>
#include "node-zinnia.h"
#include "node-zinnia-result.h"

using namespace v8;

Result::Result() {};
Result::~Result() {
  delete result_;
};

Persistent<Function> Result::constructor;

void Result::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Result"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("value"),
      FunctionTemplate::New(GetValue)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("score"),
      FunctionTemplate::New(Score)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("size"),
      FunctionTemplate::New(Size)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Result::New(const Arguments& args) {
  HandleScope scope;

  Result* obj = new Result();
  obj->result_ = static_cast<zinnia::Result*>(External::Unwrap(args[0]));
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> Result::NewInstance(const Arguments& args) {
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Handle<Value> Result::GetValue(const Arguments& args) {
  HandleScope scope;

  const char* ret = NULL;
  if(args.Length()==1 && args[0]->IsNumber()){
    Result* obj = ObjectWrap::Unwrap<Result>(args.This());

    ret = obj->result_->value(args[0]->NumberValue());
  }else{
    return ThrowException(
        Exception::Error(String::New("value argument require index.")));
  }

  return scope.Close(String::New(ret));
}

Handle<Value> Result::Score(const Arguments& args) {
  HandleScope scope;

  float ret = 0;
  if(args.Length()==1 && args[0]->IsNumber()){
    Result* obj = ObjectWrap::Unwrap<Result>(args.This());

    ret = obj->result_->score(args[0]->NumberValue());
  }else{
    return ThrowException(
        Exception::Error(String::New("score argument require index.")));
  }

  return scope.Close(Number::New(ret));
}

Handle<Value> Result::Size(const Arguments& args) {
  HandleScope scope;

  Result* obj = ObjectWrap::Unwrap<Result>(args.This());

  return scope.Close(Number::New(obj->result_->size()));
}

