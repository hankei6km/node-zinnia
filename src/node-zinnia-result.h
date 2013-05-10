/**
 * @file
 * Result クラス.
 * @author hankei6km
 */
#ifndef NODE_ZINNIA_RESULT_H
#define NODE_ZINNIA_RESULT_H 

#include <node.h>
#include <v8.h>
#include <zinnia.h>

class Result : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
  static v8::Handle<v8::Value> Size(const v8::Arguments& args);

 private:
  Result();
  ~Result();

  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  zinnia::Result *result_;
};

#endif

