/**
 * @file
 * Recognizerクラス.
 * @author hankei6km
 */
#include <node.h>
#include <uv.h>
#include <v8.h>
#include <zinnia.h>
#include "node-zinnia.h"
#include "node-zinnia-character.h"
#include "node-zinnia-recognizer.h"
#include "node-zinnia-result.h"

using namespace v8;

static Persistent<Function> result_constructor = Persistent<Function>::New(FunctionTemplate::New(Result::NewInstance)->GetFunction());

// http://kkaefer.github.io/node-cpp-modules/
struct Baton {
  uv_work_t request;
  Persistent<Function> callback;

  // Custom data
  zinnia::Recognizer *recognizer;
  zinnia::Character *character;
  size_t nbest;
  zinnia::Result *result;
};

Recognizer::Recognizer() {};
Recognizer::~Recognizer() {
  delete recognizer_;
};

Persistent<Function> Recognizer::constructor;

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
  tpl->PrototypeTemplate()->Set(String::NewSymbol("classifySync"),
      FunctionTemplate::New(ClassifySync)->GetFunction());

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

void AsyncWork(uv_work_t* req) {
  Baton* baton = static_cast<Baton*>(req->data);

  baton->result = baton->recognizer->classify(*(baton->character), baton->nbest);
}

void AsyncAfter(uv_work_t* req) {
  HandleScope scope;
  Baton* baton = static_cast<Baton*>(req->data);

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { External::New(baton->result) };
  Local<Object> instance = result_constructor->NewInstance(argc, argv);

  const unsigned argc_cb = 1;
  Local<Value> argv_cb[argc_cb] = { instance };
  baton->callback->Call(Context::GetCurrent()->Global(), argc_cb, argv_cb);

  baton->callback.Dispose();
  delete baton;
}

void AsyncAfter(uv_work_t* req, int status) {
  AsyncAfter(req);
}

Handle<Value> Recognizer::Classify(const Arguments& args) {
  HandleScope scope;

  if(args.Length()==3 && args[0]->IsObject() && args[1]->IsNumber() && args[2]->IsFunction()){
    Recognizer* obj = ObjectWrap::Unwrap<Recognizer>(args.This());

    Character* c = node::ObjectWrap::Unwrap<Character>(
      args[0]->ToObject());

    Local<Function> callback = Local<Function>::Cast(args[2]);

    Baton* baton = new Baton();
    baton->request.data = baton;
    baton->recognizer = obj->recognizer_;
    baton->character = c->character();
    baton->nbest = args[1]->NumberValue();
    baton->callback = Persistent<Function>::New(callback);

    uv_queue_work(uv_default_loop(), &baton->request,
        AsyncWork, AsyncAfter);

  }else{
    return ThrowException(
        Exception::Error(String::New("classify argument require character, nbest, callback.")));
  }

  return scope.Close(Undefined());
}

Handle<Value> Recognizer::ClassifySync(const Arguments& args) {
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
    instance = result_constructor->NewInstance(argc, argv);
  }else{
    return ThrowException(
        Exception::Error(String::New("classify argument require character, nbest.")));
  }

  return scope.Close(instance);
}
