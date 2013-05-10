/**
 * @file
 * Character クラス.
 * @author hankei6km
 */
#ifndef NODE_ZINNIA_CHARACTER_H
#define NODE_ZINNIA_CHARACTER_H 

#include <node.h>
#include <v8.h>
#include <zinnia.h>

class Character : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
  static v8::Handle<v8::Value> Clear(const v8::Arguments& args);

 private:
  Character();
  ~Character();

  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  zinnia::Character *character_;
};

#endif

