// Copyright (c) 2015 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef XWALK_EXTENSIONS_PUBLIC_XW_EXTENSION_BINARYMESSAGE_H_
#define XWALK_EXTENSIONS_PUBLIC_XW_EXTENSION_BINARYMESSAGE_H_

#ifndef XWALK_EXTENSIONS_PUBLIC_XW_EXTENSION_H_
#error "You should include XW_Extension.h before this file"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define XW_BINARY_MESSAGING_INTERFACE_1 "XW_BinaryMessagingInterface_1"
#define XW_BINARY_MESSAGING_INTERFACE XW_BINARY_MESSAGING_INTERFACE_1

typedef void (*XW_HandleBinaryMessageCallback)(XW_Instance instance,
                                               const char* message,
                                               const size_t size);

struct XW_BinaryMessagingInterface_1 {
  // Register a callback to be called when the JavaScript code associated
  // with the extension posts a message. Note that the callback will be called
  // with the XW_Instance that posted the message as well as the message
  // contents.
  void (*Register)(XW_Extension extension,
                   XW_HandleBinaryMessageCallback handle_message);

  // Post a message to the web content associated with the instance. To
  // receive this message the extension's JavaScript code should set a
  // listener using extension.setBinaryMessageListener() function.
  //
  // This function is thread-safe and can be called until the instance is
  // destroyed.
  void (*PostMessage)(XW_Instance instance, const char* message, size_t size);
};

typedef struct XW_BinaryMessagingInterface_1 XW_BinaryMessagingInterface;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // XWALK_EXTENSIONS_PUBLIC_XW_EXTENSION_BINARYMESSAGE_H_
