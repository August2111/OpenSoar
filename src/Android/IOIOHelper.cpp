// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "IOIOHelper.hpp"
#include "PortBridge.hpp"
#include "java/Class.hxx"
#include "java/Env.hxx"

Java::TrivialClass IOIOHelper::cls;
jmethodID IOIOHelper::ctor,
  IOIOHelper::openUart_method,
  IOIOHelper::shutdown_method;

bool
IOIOHelper::Initialise(JNIEnv *env)
{
  assert(!cls.IsDefined());
  assert(env != nullptr);

  if (!cls.FindOptional(env, "de/opensoar/IOIOHelper"))
    return false;

  ctor = env->GetMethodID(cls, "<init>", "()V");
  if (Java::DiscardException(env)) {
    /* need to check for Java exceptions again because the first
       method lookup initializes the Java class */
    cls.Clear(env);
    return false;
  }

  openUart_method = env->GetMethodID(cls, "openUart",
                                     "(II)Lde/opensoar/AndroidPort;");
  shutdown_method = env->GetMethodID(cls, "shutdown", "()V");

  return true;
}

void
IOIOHelper::Deinitialise(JNIEnv *env)
{
  cls.ClearOptional(env);
}

PortBridge *
IOIOHelper::openUart(JNIEnv *env, unsigned ID, unsigned baud)
{
  auto obj = Java::CallObjectMethodRethrow(env, Get(), openUart_method,
                                           ID, (int)baud);
  if (obj == nullptr)
    return nullptr;

  return new PortBridge(env, obj);
}

IOIOHelper::IOIOHelper(JNIEnv *env)
  :Java::GlobalObject(Java::NewObjectRethrow(env, cls, ctor))
{
}
