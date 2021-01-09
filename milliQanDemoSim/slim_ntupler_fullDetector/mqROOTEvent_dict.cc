// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME mqROOTEvent_dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "../include/mqGammaTrack.hh"
#include "../include/mqNeutronTrack.hh"
#include "../include/mqMuonTrack.hh"
#include "../include/mqElectronTrack.hh"
#include "../include/mqMCPTrack.hh"
#include "../include/mqPhotonTrack.hh"
#include "../include/mqPMTRHit.hh"
#include "../include/mqScintRHit.hh"
#include "../include/mqROOTEvent.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_mqGammaTrack(void *p = 0);
   static void *newArray_mqGammaTrack(Long_t size, void *p);
   static void delete_mqGammaTrack(void *p);
   static void deleteArray_mqGammaTrack(void *p);
   static void destruct_mqGammaTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqGammaTrack*)
   {
      ::mqGammaTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqGammaTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqGammaTrack", ::mqGammaTrack::Class_Version(), "../include/mqGammaTrack.hh", 22,
                  typeid(::mqGammaTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqGammaTrack::Dictionary, isa_proxy, 4,
                  sizeof(::mqGammaTrack) );
      instance.SetNew(&new_mqGammaTrack);
      instance.SetNewArray(&newArray_mqGammaTrack);
      instance.SetDelete(&delete_mqGammaTrack);
      instance.SetDeleteArray(&deleteArray_mqGammaTrack);
      instance.SetDestructor(&destruct_mqGammaTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqGammaTrack*)
   {
      return GenerateInitInstanceLocal((::mqGammaTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqGammaTrack*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_mqNeutronTrack(void *p = 0);
   static void *newArray_mqNeutronTrack(Long_t size, void *p);
   static void delete_mqNeutronTrack(void *p);
   static void deleteArray_mqNeutronTrack(void *p);
   static void destruct_mqNeutronTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqNeutronTrack*)
   {
      ::mqNeutronTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqNeutronTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqNeutronTrack", ::mqNeutronTrack::Class_Version(), "../include/mqNeutronTrack.hh", 17,
                  typeid(::mqNeutronTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqNeutronTrack::Dictionary, isa_proxy, 4,
                  sizeof(::mqNeutronTrack) );
      instance.SetNew(&new_mqNeutronTrack);
      instance.SetNewArray(&newArray_mqNeutronTrack);
      instance.SetDelete(&delete_mqNeutronTrack);
      instance.SetDeleteArray(&deleteArray_mqNeutronTrack);
      instance.SetDestructor(&destruct_mqNeutronTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqNeutronTrack*)
   {
      return GenerateInitInstanceLocal((::mqNeutronTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqNeutronTrack*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_mqMuonTrack(void *p = 0);
   static void *newArray_mqMuonTrack(Long_t size, void *p);
   static void delete_mqMuonTrack(void *p);
   static void deleteArray_mqMuonTrack(void *p);
   static void destruct_mqMuonTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqMuonTrack*)
   {
      ::mqMuonTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqMuonTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqMuonTrack", ::mqMuonTrack::Class_Version(), "../include/mqMuonTrack.hh", 17,
                  typeid(::mqMuonTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqMuonTrack::Dictionary, isa_proxy, 4,
                  sizeof(::mqMuonTrack) );
      instance.SetNew(&new_mqMuonTrack);
      instance.SetNewArray(&newArray_mqMuonTrack);
      instance.SetDelete(&delete_mqMuonTrack);
      instance.SetDeleteArray(&deleteArray_mqMuonTrack);
      instance.SetDestructor(&destruct_mqMuonTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqMuonTrack*)
   {
      return GenerateInitInstanceLocal((::mqMuonTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqMuonTrack*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_mqElectronTrack(void *p = 0);
   static void *newArray_mqElectronTrack(Long_t size, void *p);
   static void delete_mqElectronTrack(void *p);
   static void deleteArray_mqElectronTrack(void *p);
   static void destruct_mqElectronTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqElectronTrack*)
   {
      ::mqElectronTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqElectronTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqElectronTrack", ::mqElectronTrack::Class_Version(), "../include/mqElectronTrack.hh", 17,
                  typeid(::mqElectronTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqElectronTrack::Dictionary, isa_proxy, 4,
                  sizeof(::mqElectronTrack) );
      instance.SetNew(&new_mqElectronTrack);
      instance.SetNewArray(&newArray_mqElectronTrack);
      instance.SetDelete(&delete_mqElectronTrack);
      instance.SetDeleteArray(&deleteArray_mqElectronTrack);
      instance.SetDestructor(&destruct_mqElectronTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqElectronTrack*)
   {
      return GenerateInitInstanceLocal((::mqElectronTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqElectronTrack*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_mqMCPTrack(void *p = 0);
   static void *newArray_mqMCPTrack(Long_t size, void *p);
   static void delete_mqMCPTrack(void *p);
   static void deleteArray_mqMCPTrack(void *p);
   static void destruct_mqMCPTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqMCPTrack*)
   {
      ::mqMCPTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqMCPTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqMCPTrack", ::mqMCPTrack::Class_Version(), "../include/mqMCPTrack.hh", 17,
                  typeid(::mqMCPTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqMCPTrack::Dictionary, isa_proxy, 4,
                  sizeof(::mqMCPTrack) );
      instance.SetNew(&new_mqMCPTrack);
      instance.SetNewArray(&newArray_mqMCPTrack);
      instance.SetDelete(&delete_mqMCPTrack);
      instance.SetDeleteArray(&deleteArray_mqMCPTrack);
      instance.SetDestructor(&destruct_mqMCPTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqMCPTrack*)
   {
      return GenerateInitInstanceLocal((::mqMCPTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqMCPTrack*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_mqPhotonTrack(void *p = 0);
   static void *newArray_mqPhotonTrack(Long_t size, void *p);
   static void delete_mqPhotonTrack(void *p);
   static void deleteArray_mqPhotonTrack(void *p);
   static void destruct_mqPhotonTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqPhotonTrack*)
   {
      ::mqPhotonTrack *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqPhotonTrack >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqPhotonTrack", ::mqPhotonTrack::Class_Version(), "../include/mqPhotonTrack.hh", 17,
                  typeid(::mqPhotonTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqPhotonTrack::Dictionary, isa_proxy, 4,
                  sizeof(::mqPhotonTrack) );
      instance.SetNew(&new_mqPhotonTrack);
      instance.SetNewArray(&newArray_mqPhotonTrack);
      instance.SetDelete(&delete_mqPhotonTrack);
      instance.SetDeleteArray(&deleteArray_mqPhotonTrack);
      instance.SetDestructor(&destruct_mqPhotonTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqPhotonTrack*)
   {
      return GenerateInitInstanceLocal((::mqPhotonTrack*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqPhotonTrack*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_mqPMTRHit(void *p = 0);
   static void *newArray_mqPMTRHit(Long_t size, void *p);
   static void delete_mqPMTRHit(void *p);
   static void deleteArray_mqPMTRHit(void *p);
   static void destruct_mqPMTRHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqPMTRHit*)
   {
      ::mqPMTRHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqPMTRHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqPMTRHit", ::mqPMTRHit::Class_Version(), "../include/mqPMTRHit.hh", 18,
                  typeid(::mqPMTRHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqPMTRHit::Dictionary, isa_proxy, 4,
                  sizeof(::mqPMTRHit) );
      instance.SetNew(&new_mqPMTRHit);
      instance.SetNewArray(&newArray_mqPMTRHit);
      instance.SetDelete(&delete_mqPMTRHit);
      instance.SetDeleteArray(&deleteArray_mqPMTRHit);
      instance.SetDestructor(&destruct_mqPMTRHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqPMTRHit*)
   {
      return GenerateInitInstanceLocal((::mqPMTRHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqPMTRHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_mqScintRHit(void *p = 0);
   static void *newArray_mqScintRHit(Long_t size, void *p);
   static void delete_mqScintRHit(void *p);
   static void deleteArray_mqScintRHit(void *p);
   static void destruct_mqScintRHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqScintRHit*)
   {
      ::mqScintRHit *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqScintRHit >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqScintRHit", ::mqScintRHit::Class_Version(), "../include/mqScintRHit.hh", 16,
                  typeid(::mqScintRHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqScintRHit::Dictionary, isa_proxy, 4,
                  sizeof(::mqScintRHit) );
      instance.SetNew(&new_mqScintRHit);
      instance.SetNewArray(&newArray_mqScintRHit);
      instance.SetDelete(&delete_mqScintRHit);
      instance.SetDeleteArray(&deleteArray_mqScintRHit);
      instance.SetDestructor(&destruct_mqScintRHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqScintRHit*)
   {
      return GenerateInitInstanceLocal((::mqScintRHit*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqScintRHit*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_mqROOTEvent(void *p = 0);
   static void *newArray_mqROOTEvent(Long_t size, void *p);
   static void delete_mqROOTEvent(void *p);
   static void deleteArray_mqROOTEvent(void *p);
   static void destruct_mqROOTEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::mqROOTEvent*)
   {
      ::mqROOTEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::mqROOTEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("mqROOTEvent", ::mqROOTEvent::Class_Version(), "../include/mqROOTEvent.hh", 25,
                  typeid(::mqROOTEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::mqROOTEvent::Dictionary, isa_proxy, 4,
                  sizeof(::mqROOTEvent) );
      instance.SetNew(&new_mqROOTEvent);
      instance.SetNewArray(&newArray_mqROOTEvent);
      instance.SetDelete(&delete_mqROOTEvent);
      instance.SetDeleteArray(&deleteArray_mqROOTEvent);
      instance.SetDestructor(&destruct_mqROOTEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::mqROOTEvent*)
   {
      return GenerateInitInstanceLocal((::mqROOTEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::mqROOTEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr mqGammaTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqGammaTrack::Class_Name()
{
   return "mqGammaTrack";
}

//______________________________________________________________________________
const char *mqGammaTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqGammaTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqGammaTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqGammaTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqGammaTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqGammaTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqGammaTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqGammaTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr mqNeutronTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqNeutronTrack::Class_Name()
{
   return "mqNeutronTrack";
}

//______________________________________________________________________________
const char *mqNeutronTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqNeutronTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqNeutronTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqNeutronTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqNeutronTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqNeutronTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqNeutronTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqNeutronTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr mqMuonTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqMuonTrack::Class_Name()
{
   return "mqMuonTrack";
}

//______________________________________________________________________________
const char *mqMuonTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqMuonTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqMuonTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqMuonTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqMuonTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqMuonTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqMuonTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqMuonTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr mqElectronTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqElectronTrack::Class_Name()
{
   return "mqElectronTrack";
}

//______________________________________________________________________________
const char *mqElectronTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqElectronTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqElectronTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqElectronTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqElectronTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqElectronTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqElectronTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqElectronTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr mqMCPTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqMCPTrack::Class_Name()
{
   return "mqMCPTrack";
}

//______________________________________________________________________________
const char *mqMCPTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqMCPTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqMCPTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqMCPTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqMCPTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqMCPTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqMCPTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqMCPTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr mqPhotonTrack::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqPhotonTrack::Class_Name()
{
   return "mqPhotonTrack";
}

//______________________________________________________________________________
const char *mqPhotonTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqPhotonTrack*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqPhotonTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqPhotonTrack*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqPhotonTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqPhotonTrack*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqPhotonTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqPhotonTrack*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr mqPMTRHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqPMTRHit::Class_Name()
{
   return "mqPMTRHit";
}

//______________________________________________________________________________
const char *mqPMTRHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqPMTRHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqPMTRHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqPMTRHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqPMTRHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqPMTRHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqPMTRHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqPMTRHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr mqScintRHit::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqScintRHit::Class_Name()
{
   return "mqScintRHit";
}

//______________________________________________________________________________
const char *mqScintRHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqScintRHit*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqScintRHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqScintRHit*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqScintRHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqScintRHit*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqScintRHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqScintRHit*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr mqROOTEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *mqROOTEvent::Class_Name()
{
   return "mqROOTEvent";
}

//______________________________________________________________________________
const char *mqROOTEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqROOTEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int mqROOTEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::mqROOTEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *mqROOTEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqROOTEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *mqROOTEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::mqROOTEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void mqGammaTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqGammaTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqGammaTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqGammaTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqGammaTrack(void *p) {
      return  p ? new(p) ::mqGammaTrack : new ::mqGammaTrack;
   }
   static void *newArray_mqGammaTrack(Long_t nElements, void *p) {
      return p ? new(p) ::mqGammaTrack[nElements] : new ::mqGammaTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqGammaTrack(void *p) {
      delete ((::mqGammaTrack*)p);
   }
   static void deleteArray_mqGammaTrack(void *p) {
      delete [] ((::mqGammaTrack*)p);
   }
   static void destruct_mqGammaTrack(void *p) {
      typedef ::mqGammaTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqGammaTrack

//______________________________________________________________________________
void mqNeutronTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqNeutronTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqNeutronTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqNeutronTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqNeutronTrack(void *p) {
      return  p ? new(p) ::mqNeutronTrack : new ::mqNeutronTrack;
   }
   static void *newArray_mqNeutronTrack(Long_t nElements, void *p) {
      return p ? new(p) ::mqNeutronTrack[nElements] : new ::mqNeutronTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqNeutronTrack(void *p) {
      delete ((::mqNeutronTrack*)p);
   }
   static void deleteArray_mqNeutronTrack(void *p) {
      delete [] ((::mqNeutronTrack*)p);
   }
   static void destruct_mqNeutronTrack(void *p) {
      typedef ::mqNeutronTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqNeutronTrack

//______________________________________________________________________________
void mqMuonTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqMuonTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqMuonTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqMuonTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqMuonTrack(void *p) {
      return  p ? new(p) ::mqMuonTrack : new ::mqMuonTrack;
   }
   static void *newArray_mqMuonTrack(Long_t nElements, void *p) {
      return p ? new(p) ::mqMuonTrack[nElements] : new ::mqMuonTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqMuonTrack(void *p) {
      delete ((::mqMuonTrack*)p);
   }
   static void deleteArray_mqMuonTrack(void *p) {
      delete [] ((::mqMuonTrack*)p);
   }
   static void destruct_mqMuonTrack(void *p) {
      typedef ::mqMuonTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqMuonTrack

//______________________________________________________________________________
void mqElectronTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqElectronTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqElectronTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqElectronTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqElectronTrack(void *p) {
      return  p ? new(p) ::mqElectronTrack : new ::mqElectronTrack;
   }
   static void *newArray_mqElectronTrack(Long_t nElements, void *p) {
      return p ? new(p) ::mqElectronTrack[nElements] : new ::mqElectronTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqElectronTrack(void *p) {
      delete ((::mqElectronTrack*)p);
   }
   static void deleteArray_mqElectronTrack(void *p) {
      delete [] ((::mqElectronTrack*)p);
   }
   static void destruct_mqElectronTrack(void *p) {
      typedef ::mqElectronTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqElectronTrack

//______________________________________________________________________________
void mqMCPTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqMCPTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqMCPTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqMCPTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqMCPTrack(void *p) {
      return  p ? new(p) ::mqMCPTrack : new ::mqMCPTrack;
   }
   static void *newArray_mqMCPTrack(Long_t nElements, void *p) {
      return p ? new(p) ::mqMCPTrack[nElements] : new ::mqMCPTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqMCPTrack(void *p) {
      delete ((::mqMCPTrack*)p);
   }
   static void deleteArray_mqMCPTrack(void *p) {
      delete [] ((::mqMCPTrack*)p);
   }
   static void destruct_mqMCPTrack(void *p) {
      typedef ::mqMCPTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqMCPTrack

//______________________________________________________________________________
void mqPhotonTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqPhotonTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqPhotonTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqPhotonTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqPhotonTrack(void *p) {
      return  p ? new(p) ::mqPhotonTrack : new ::mqPhotonTrack;
   }
   static void *newArray_mqPhotonTrack(Long_t nElements, void *p) {
      return p ? new(p) ::mqPhotonTrack[nElements] : new ::mqPhotonTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqPhotonTrack(void *p) {
      delete ((::mqPhotonTrack*)p);
   }
   static void deleteArray_mqPhotonTrack(void *p) {
      delete [] ((::mqPhotonTrack*)p);
   }
   static void destruct_mqPhotonTrack(void *p) {
      typedef ::mqPhotonTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqPhotonTrack

//______________________________________________________________________________
void mqPMTRHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqPMTRHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqPMTRHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqPMTRHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqPMTRHit(void *p) {
      return  p ? new(p) ::mqPMTRHit : new ::mqPMTRHit;
   }
   static void *newArray_mqPMTRHit(Long_t nElements, void *p) {
      return p ? new(p) ::mqPMTRHit[nElements] : new ::mqPMTRHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqPMTRHit(void *p) {
      delete ((::mqPMTRHit*)p);
   }
   static void deleteArray_mqPMTRHit(void *p) {
      delete [] ((::mqPMTRHit*)p);
   }
   static void destruct_mqPMTRHit(void *p) {
      typedef ::mqPMTRHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqPMTRHit

//______________________________________________________________________________
void mqScintRHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqScintRHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqScintRHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqScintRHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqScintRHit(void *p) {
      return  p ? new(p) ::mqScintRHit : new ::mqScintRHit;
   }
   static void *newArray_mqScintRHit(Long_t nElements, void *p) {
      return p ? new(p) ::mqScintRHit[nElements] : new ::mqScintRHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqScintRHit(void *p) {
      delete ((::mqScintRHit*)p);
   }
   static void deleteArray_mqScintRHit(void *p) {
      delete [] ((::mqScintRHit*)p);
   }
   static void destruct_mqScintRHit(void *p) {
      typedef ::mqScintRHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqScintRHit

//______________________________________________________________________________
void mqROOTEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class mqROOTEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(mqROOTEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(mqROOTEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_mqROOTEvent(void *p) {
      return  p ? new(p) ::mqROOTEvent : new ::mqROOTEvent;
   }
   static void *newArray_mqROOTEvent(Long_t nElements, void *p) {
      return p ? new(p) ::mqROOTEvent[nElements] : new ::mqROOTEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_mqROOTEvent(void *p) {
      delete ((::mqROOTEvent*)p);
   }
   static void deleteArray_mqROOTEvent(void *p) {
      delete [] ((::mqROOTEvent*)p);
   }
   static void destruct_mqROOTEvent(void *p) {
      typedef ::mqROOTEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::mqROOTEvent

namespace ROOT {
   static TClass *vectorlEmqScintRHitmUgR_Dictionary();
   static void vectorlEmqScintRHitmUgR_TClassManip(TClass*);
   static void *new_vectorlEmqScintRHitmUgR(void *p = 0);
   static void *newArray_vectorlEmqScintRHitmUgR(Long_t size, void *p);
   static void delete_vectorlEmqScintRHitmUgR(void *p);
   static void deleteArray_vectorlEmqScintRHitmUgR(void *p);
   static void destruct_vectorlEmqScintRHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mqScintRHit*>*)
   {
      vector<mqScintRHit*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mqScintRHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mqScintRHit*>", -2, "vector", 216,
                  typeid(vector<mqScintRHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmqScintRHitmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mqScintRHit*>) );
      instance.SetNew(&new_vectorlEmqScintRHitmUgR);
      instance.SetNewArray(&newArray_vectorlEmqScintRHitmUgR);
      instance.SetDelete(&delete_vectorlEmqScintRHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmqScintRHitmUgR);
      instance.SetDestructor(&destruct_vectorlEmqScintRHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mqScintRHit*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<mqScintRHit*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmqScintRHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mqScintRHit*>*)0x0)->GetClass();
      vectorlEmqScintRHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmqScintRHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmqScintRHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqScintRHit*> : new vector<mqScintRHit*>;
   }
   static void *newArray_vectorlEmqScintRHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqScintRHit*>[nElements] : new vector<mqScintRHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmqScintRHitmUgR(void *p) {
      delete ((vector<mqScintRHit*>*)p);
   }
   static void deleteArray_vectorlEmqScintRHitmUgR(void *p) {
      delete [] ((vector<mqScintRHit*>*)p);
   }
   static void destruct_vectorlEmqScintRHitmUgR(void *p) {
      typedef vector<mqScintRHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mqScintRHit*>

namespace ROOT {
   static TClass *vectorlEmqPhotonTrackmUgR_Dictionary();
   static void vectorlEmqPhotonTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEmqPhotonTrackmUgR(void *p = 0);
   static void *newArray_vectorlEmqPhotonTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEmqPhotonTrackmUgR(void *p);
   static void deleteArray_vectorlEmqPhotonTrackmUgR(void *p);
   static void destruct_vectorlEmqPhotonTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mqPhotonTrack*>*)
   {
      vector<mqPhotonTrack*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mqPhotonTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mqPhotonTrack*>", -2, "vector", 216,
                  typeid(vector<mqPhotonTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmqPhotonTrackmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mqPhotonTrack*>) );
      instance.SetNew(&new_vectorlEmqPhotonTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEmqPhotonTrackmUgR);
      instance.SetDelete(&delete_vectorlEmqPhotonTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmqPhotonTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEmqPhotonTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mqPhotonTrack*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<mqPhotonTrack*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmqPhotonTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mqPhotonTrack*>*)0x0)->GetClass();
      vectorlEmqPhotonTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmqPhotonTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmqPhotonTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqPhotonTrack*> : new vector<mqPhotonTrack*>;
   }
   static void *newArray_vectorlEmqPhotonTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqPhotonTrack*>[nElements] : new vector<mqPhotonTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmqPhotonTrackmUgR(void *p) {
      delete ((vector<mqPhotonTrack*>*)p);
   }
   static void deleteArray_vectorlEmqPhotonTrackmUgR(void *p) {
      delete [] ((vector<mqPhotonTrack*>*)p);
   }
   static void destruct_vectorlEmqPhotonTrackmUgR(void *p) {
      typedef vector<mqPhotonTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mqPhotonTrack*>

namespace ROOT {
   static TClass *vectorlEmqPMTRHitmUgR_Dictionary();
   static void vectorlEmqPMTRHitmUgR_TClassManip(TClass*);
   static void *new_vectorlEmqPMTRHitmUgR(void *p = 0);
   static void *newArray_vectorlEmqPMTRHitmUgR(Long_t size, void *p);
   static void delete_vectorlEmqPMTRHitmUgR(void *p);
   static void deleteArray_vectorlEmqPMTRHitmUgR(void *p);
   static void destruct_vectorlEmqPMTRHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mqPMTRHit*>*)
   {
      vector<mqPMTRHit*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mqPMTRHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mqPMTRHit*>", -2, "vector", 216,
                  typeid(vector<mqPMTRHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmqPMTRHitmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mqPMTRHit*>) );
      instance.SetNew(&new_vectorlEmqPMTRHitmUgR);
      instance.SetNewArray(&newArray_vectorlEmqPMTRHitmUgR);
      instance.SetDelete(&delete_vectorlEmqPMTRHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmqPMTRHitmUgR);
      instance.SetDestructor(&destruct_vectorlEmqPMTRHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mqPMTRHit*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<mqPMTRHit*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmqPMTRHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mqPMTRHit*>*)0x0)->GetClass();
      vectorlEmqPMTRHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmqPMTRHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmqPMTRHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqPMTRHit*> : new vector<mqPMTRHit*>;
   }
   static void *newArray_vectorlEmqPMTRHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqPMTRHit*>[nElements] : new vector<mqPMTRHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmqPMTRHitmUgR(void *p) {
      delete ((vector<mqPMTRHit*>*)p);
   }
   static void deleteArray_vectorlEmqPMTRHitmUgR(void *p) {
      delete [] ((vector<mqPMTRHit*>*)p);
   }
   static void destruct_vectorlEmqPMTRHitmUgR(void *p) {
      typedef vector<mqPMTRHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mqPMTRHit*>

namespace ROOT {
   static TClass *vectorlEmqNeutronTrackmUgR_Dictionary();
   static void vectorlEmqNeutronTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEmqNeutronTrackmUgR(void *p = 0);
   static void *newArray_vectorlEmqNeutronTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEmqNeutronTrackmUgR(void *p);
   static void deleteArray_vectorlEmqNeutronTrackmUgR(void *p);
   static void destruct_vectorlEmqNeutronTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mqNeutronTrack*>*)
   {
      vector<mqNeutronTrack*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mqNeutronTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mqNeutronTrack*>", -2, "vector", 216,
                  typeid(vector<mqNeutronTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmqNeutronTrackmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mqNeutronTrack*>) );
      instance.SetNew(&new_vectorlEmqNeutronTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEmqNeutronTrackmUgR);
      instance.SetDelete(&delete_vectorlEmqNeutronTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmqNeutronTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEmqNeutronTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mqNeutronTrack*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<mqNeutronTrack*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmqNeutronTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mqNeutronTrack*>*)0x0)->GetClass();
      vectorlEmqNeutronTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmqNeutronTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmqNeutronTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqNeutronTrack*> : new vector<mqNeutronTrack*>;
   }
   static void *newArray_vectorlEmqNeutronTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqNeutronTrack*>[nElements] : new vector<mqNeutronTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmqNeutronTrackmUgR(void *p) {
      delete ((vector<mqNeutronTrack*>*)p);
   }
   static void deleteArray_vectorlEmqNeutronTrackmUgR(void *p) {
      delete [] ((vector<mqNeutronTrack*>*)p);
   }
   static void destruct_vectorlEmqNeutronTrackmUgR(void *p) {
      typedef vector<mqNeutronTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mqNeutronTrack*>

namespace ROOT {
   static TClass *vectorlEmqMuonTrackmUgR_Dictionary();
   static void vectorlEmqMuonTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEmqMuonTrackmUgR(void *p = 0);
   static void *newArray_vectorlEmqMuonTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEmqMuonTrackmUgR(void *p);
   static void deleteArray_vectorlEmqMuonTrackmUgR(void *p);
   static void destruct_vectorlEmqMuonTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mqMuonTrack*>*)
   {
      vector<mqMuonTrack*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mqMuonTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mqMuonTrack*>", -2, "vector", 216,
                  typeid(vector<mqMuonTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmqMuonTrackmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mqMuonTrack*>) );
      instance.SetNew(&new_vectorlEmqMuonTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEmqMuonTrackmUgR);
      instance.SetDelete(&delete_vectorlEmqMuonTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmqMuonTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEmqMuonTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mqMuonTrack*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<mqMuonTrack*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmqMuonTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mqMuonTrack*>*)0x0)->GetClass();
      vectorlEmqMuonTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmqMuonTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmqMuonTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqMuonTrack*> : new vector<mqMuonTrack*>;
   }
   static void *newArray_vectorlEmqMuonTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqMuonTrack*>[nElements] : new vector<mqMuonTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmqMuonTrackmUgR(void *p) {
      delete ((vector<mqMuonTrack*>*)p);
   }
   static void deleteArray_vectorlEmqMuonTrackmUgR(void *p) {
      delete [] ((vector<mqMuonTrack*>*)p);
   }
   static void destruct_vectorlEmqMuonTrackmUgR(void *p) {
      typedef vector<mqMuonTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mqMuonTrack*>

namespace ROOT {
   static TClass *vectorlEmqMCPTrackmUgR_Dictionary();
   static void vectorlEmqMCPTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEmqMCPTrackmUgR(void *p = 0);
   static void *newArray_vectorlEmqMCPTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEmqMCPTrackmUgR(void *p);
   static void deleteArray_vectorlEmqMCPTrackmUgR(void *p);
   static void destruct_vectorlEmqMCPTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mqMCPTrack*>*)
   {
      vector<mqMCPTrack*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mqMCPTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mqMCPTrack*>", -2, "vector", 216,
                  typeid(vector<mqMCPTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmqMCPTrackmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mqMCPTrack*>) );
      instance.SetNew(&new_vectorlEmqMCPTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEmqMCPTrackmUgR);
      instance.SetDelete(&delete_vectorlEmqMCPTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmqMCPTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEmqMCPTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mqMCPTrack*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<mqMCPTrack*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmqMCPTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mqMCPTrack*>*)0x0)->GetClass();
      vectorlEmqMCPTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmqMCPTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmqMCPTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqMCPTrack*> : new vector<mqMCPTrack*>;
   }
   static void *newArray_vectorlEmqMCPTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqMCPTrack*>[nElements] : new vector<mqMCPTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmqMCPTrackmUgR(void *p) {
      delete ((vector<mqMCPTrack*>*)p);
   }
   static void deleteArray_vectorlEmqMCPTrackmUgR(void *p) {
      delete [] ((vector<mqMCPTrack*>*)p);
   }
   static void destruct_vectorlEmqMCPTrackmUgR(void *p) {
      typedef vector<mqMCPTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mqMCPTrack*>

namespace ROOT {
   static TClass *vectorlEmqGammaTrackmUgR_Dictionary();
   static void vectorlEmqGammaTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEmqGammaTrackmUgR(void *p = 0);
   static void *newArray_vectorlEmqGammaTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEmqGammaTrackmUgR(void *p);
   static void deleteArray_vectorlEmqGammaTrackmUgR(void *p);
   static void destruct_vectorlEmqGammaTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mqGammaTrack*>*)
   {
      vector<mqGammaTrack*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mqGammaTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mqGammaTrack*>", -2, "vector", 216,
                  typeid(vector<mqGammaTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmqGammaTrackmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mqGammaTrack*>) );
      instance.SetNew(&new_vectorlEmqGammaTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEmqGammaTrackmUgR);
      instance.SetDelete(&delete_vectorlEmqGammaTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmqGammaTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEmqGammaTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mqGammaTrack*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<mqGammaTrack*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmqGammaTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mqGammaTrack*>*)0x0)->GetClass();
      vectorlEmqGammaTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmqGammaTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmqGammaTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqGammaTrack*> : new vector<mqGammaTrack*>;
   }
   static void *newArray_vectorlEmqGammaTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqGammaTrack*>[nElements] : new vector<mqGammaTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmqGammaTrackmUgR(void *p) {
      delete ((vector<mqGammaTrack*>*)p);
   }
   static void deleteArray_vectorlEmqGammaTrackmUgR(void *p) {
      delete [] ((vector<mqGammaTrack*>*)p);
   }
   static void destruct_vectorlEmqGammaTrackmUgR(void *p) {
      typedef vector<mqGammaTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mqGammaTrack*>

namespace ROOT {
   static TClass *vectorlEmqElectronTrackmUgR_Dictionary();
   static void vectorlEmqElectronTrackmUgR_TClassManip(TClass*);
   static void *new_vectorlEmqElectronTrackmUgR(void *p = 0);
   static void *newArray_vectorlEmqElectronTrackmUgR(Long_t size, void *p);
   static void delete_vectorlEmqElectronTrackmUgR(void *p);
   static void deleteArray_vectorlEmqElectronTrackmUgR(void *p);
   static void destruct_vectorlEmqElectronTrackmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<mqElectronTrack*>*)
   {
      vector<mqElectronTrack*> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<mqElectronTrack*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<mqElectronTrack*>", -2, "vector", 216,
                  typeid(vector<mqElectronTrack*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEmqElectronTrackmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<mqElectronTrack*>) );
      instance.SetNew(&new_vectorlEmqElectronTrackmUgR);
      instance.SetNewArray(&newArray_vectorlEmqElectronTrackmUgR);
      instance.SetDelete(&delete_vectorlEmqElectronTrackmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEmqElectronTrackmUgR);
      instance.SetDestructor(&destruct_vectorlEmqElectronTrackmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<mqElectronTrack*> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<mqElectronTrack*>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEmqElectronTrackmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<mqElectronTrack*>*)0x0)->GetClass();
      vectorlEmqElectronTrackmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEmqElectronTrackmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEmqElectronTrackmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqElectronTrack*> : new vector<mqElectronTrack*>;
   }
   static void *newArray_vectorlEmqElectronTrackmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<mqElectronTrack*>[nElements] : new vector<mqElectronTrack*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEmqElectronTrackmUgR(void *p) {
      delete ((vector<mqElectronTrack*>*)p);
   }
   static void deleteArray_vectorlEmqElectronTrackmUgR(void *p) {
      delete [] ((vector<mqElectronTrack*>*)p);
   }
   static void destruct_vectorlEmqElectronTrackmUgR(void *p) {
      typedef vector<mqElectronTrack*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<mqElectronTrack*>

namespace {
  void TriggerDictionaryInitialization_mqROOTEvent_dict_Impl() {
    static const char* headers[] = {
"../include/mqGammaTrack.hh",
"../include/mqNeutronTrack.hh",
"../include/mqMuonTrack.hh",
"../include/mqElectronTrack.hh",
"../include/mqMCPTrack.hh",
"../include/mqPhotonTrack.hh",
"../include/mqPMTRHit.hh",
"../include/mqScintRHit.hh",
"../include/mqROOTEvent.hh",
0
    };
    static const char* includePaths[] = {
"/cvmfs/cms.cern.ch/slc6_amd64_gcc700/lcg/root/6.12.07-gnimlf7/include",
"/net/cms25/cms25r5/fsetti/milliQanDemoSim/slim_ntupler_fullDetector/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "mqROOTEvent_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$../include/mqGammaTrack.hh")))  mqGammaTrack;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$../include/mqNeutronTrack.hh")))  mqNeutronTrack;
class __attribute__((annotate("$clingAutoload$../include/mqMuonTrack.hh")))  mqMuonTrack;
class __attribute__((annotate("$clingAutoload$../include/mqElectronTrack.hh")))  mqElectronTrack;
class __attribute__((annotate("$clingAutoload$../include/mqMCPTrack.hh")))  mqMCPTrack;
class __attribute__((annotate("$clingAutoload$../include/mqPhotonTrack.hh")))  mqPhotonTrack;
class __attribute__((annotate("$clingAutoload$../include/mqPMTRHit.hh")))  mqPMTRHit;
class __attribute__((annotate("$clingAutoload$../include/mqScintRHit.hh")))  mqScintRHit;
class __attribute__((annotate("$clingAutoload$../include/mqROOTEvent.hh")))  mqROOTEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "mqROOTEvent_dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "../include/mqGammaTrack.hh"
#include "../include/mqNeutronTrack.hh"
#include "../include/mqMuonTrack.hh"
#include "../include/mqElectronTrack.hh"
#include "../include/mqMCPTrack.hh"
#include "../include/mqPhotonTrack.hh"
#include "../include/mqPMTRHit.hh"
#include "../include/mqScintRHit.hh"
#include "../include/mqROOTEvent.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"mqElectronTrack", payloadCode, "@",
"mqElectronTrackVector", payloadCode, "@",
"mqGammaTrack", payloadCode, "@",
"mqGammaTrackVector", payloadCode, "@",
"mqMCPTrack", payloadCode, "@",
"mqMCPTrackVector", payloadCode, "@",
"mqMuonTrack", payloadCode, "@",
"mqMuonTrackVector", payloadCode, "@",
"mqNeutronTrack", payloadCode, "@",
"mqNeutronTrackVector", payloadCode, "@",
"mqPMTRHit", payloadCode, "@",
"mqPMTRHitVector", payloadCode, "@",
"mqPhotonTrack", payloadCode, "@",
"mqPhotonTrackVector", payloadCode, "@",
"mqROOTEvent", payloadCode, "@",
"mqScintRHit", payloadCode, "@",
"mqScintRHitVector", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("mqROOTEvent_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_mqROOTEvent_dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_mqROOTEvent_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_mqROOTEvent_dict() {
  TriggerDictionaryInitialization_mqROOTEvent_dict_Impl();
}
