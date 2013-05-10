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
Character::~Character() {};

Persistent<Function> Character::constructor;

void Character::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Character"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("open"),
      FunctionTemplate::New(Clear)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Character::New(const Arguments& args) {
  HandleScope scope;

  Character* obj = new Character();
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

Handle<Value> Character::Clear(const Arguments& args) {
  HandleScope scope;

  Character* obj = ObjectWrap::Unwrap<Character>(args.This());
  obj->character_->clear();

  return scope.Close(Undefined());
}

