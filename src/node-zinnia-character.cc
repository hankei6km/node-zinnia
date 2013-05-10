/**
 * @file
 * Character クラス.
 * @author hankei6km
 */
#include <node.h>
#include <v8.h>
#include <zinnia.h>
#include "node-zinnia.h"
#include "node-zinnia-character.h"

using namespace v8;

Character::Character() {};
Character::~Character() {
  delete character_;
};

Persistent<Function> Character::constructor;

void Character::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Character"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("set_width"),
      FunctionTemplate::New(SetWidth)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("set_height"),
      FunctionTemplate::New(SetHeight)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("clear"),
      FunctionTemplate::New(Clear)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("add"),
      FunctionTemplate::New(Add)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Character::New(const Arguments& args) {
  HandleScope scope;

  Character* obj = new Character();
  obj->character_ = zinnia::Character::create();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> Character::NewInstance(const Arguments& args) {
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Handle<Value> Character::SetWidth(const Arguments& args) {
  HandleScope scope;

  if(args.Length()==1 && args[0]->IsNumber()){
    Character* obj = ObjectWrap::Unwrap<Character>(args.This());

    obj->character_->set_width(args[0]->NumberValue());
  }else{
    return ThrowException(Exception::Error(String::New("set_width argument require width.")));
  }

  return scope.Close(Undefined());
}

Handle<Value> Character::SetHeight(const Arguments& args) {
  HandleScope scope;

  if(args.Length()==1 && args[0]->IsNumber()){
    Character* obj = ObjectWrap::Unwrap<Character>(args.This());

    obj->character_->set_height(args[0]->NumberValue());
  }else{
    return ThrowException(Exception::Error(String::New("set_height argument require width.")));
  }

  return scope.Close(Undefined());
}

Handle<Value> Character::Clear(const Arguments& args) {
  HandleScope scope;

  Character* obj = ObjectWrap::Unwrap<Character>(args.This());
  obj->character_->clear();

  return scope.Close(Undefined());
}

Handle<Value> Character::Add(const Arguments& args) {
  HandleScope scope;

  if(args.Length()==3 && 
      args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber()){
    Character* obj = ObjectWrap::Unwrap<Character>(args.This());

    obj->character_->add(
        args[0]->NumberValue(),
        args[1]->NumberValue(),
        args[2]->NumberValue());
  }else{
    return ThrowException(Exception::Error(String::New("add argument require id, x, y.")));
  }

  return scope.Close(Undefined());
}
