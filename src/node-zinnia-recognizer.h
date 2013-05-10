/**
 * @file
 * Recognizer クラス.
 * @author hankei6km
 */
#ifndef NODE_ZINNIA_RECOGNIZER_H
#define NODE_ZINNIA_RECOGNIZER_H 

#include <node.h>
#include <v8.h>
#include <zinnia.h>

class Recognizer : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
  static v8::Handle<v8::Value> Open(const v8::Arguments& args);

 private:
  Recognizer();
  ~Recognizer();

  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  zinnia::Recognizer *recognizer_;
};

#endif
