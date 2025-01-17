/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_CORE_OBJECTMODEL_BASECLASS_H
#define SOFA_CORE_OBJECTMODEL_BASECLASS_H

#include <sofa/core/config.h>
#include <sofa/helper/NameDecoder.h>
#include <sofa/core/objectmodel/SPtr.h>
#include <map>

namespace sofa
{

namespace core
{

namespace objectmodel
{

class Base;
using sofa::helper::NameDecoder;

/**
 *  \brief Class hierarchy reflection base class
 *
 *  This class provides information on the class and parent classes of components.
 *  It is created by using the SOFA_CLASS macro on each new class declaration.
 *  All classes deriving from Base should use the SOFA_CLASS macro within their declaration.
 *
 */
class SOFA_CORE_API BaseClass
{
protected:
    BaseClass();
    virtual ~BaseClass();

public:


    /// @todo the names could be hashed for faster comparisons

    std::string namespaceName;
    std::string typeName;
    std::string className;
    std::string templateName;
    std::string shortName;
    helper::vector<const BaseClass*> parents;

    /// returns true iff c is a parent class of this
    bool hasParent(const BaseClass* c) const
    {
        if (*this == *c) return true;
        for (unsigned int i=0; i<parents.size(); ++i)
            if (parents[i]->hasParent(c)) return true;
        return false;
    }

    /// returns true iff a parent class of this is named parentClassName
    bool hasParent(const std::string& parentClassName) const
    {
        if (className==parentClassName) return true;
        for (unsigned int i=0; i<parents.size(); ++i)
            if (parents[i]->hasParent(parentClassName)) return true;
        return false;
    }

    bool operator==(const BaseClass& c) const
    {
        if (this == &c) return true;
        return (this->namespaceName == c.namespaceName)
                && (this->className == c.className)
                && (this->templateName == c.templateName);
    }

    bool operator!=(const BaseClass& c) const
    {
        return !((*this)==c);
    }

    virtual Base* dynamicCast(Base* obj) const = 0;
    virtual bool isInstance(Base* obj) const = 0;

    /// Helper method to decode the type name
    SOFA_ATTRIBUTE_DISABLED__CLASSNAME_INTROSPECTION()
    static std::string decodeFullName(const std::type_info& t) = delete;

    /// Helper method to decode the type name to a more readable form if possible
    SOFA_ATTRIBUTE_DISABLED__CLASSNAME_INTROSPECTION()
    static std::string decodeTypeName(const std::type_info& t) = delete;

    /// Helper method to extract the class name (removing namespaces and templates)
    SOFA_ATTRIBUTE_DISABLED__CLASSNAME_INTROSPECTION()
    static std::string decodeClassName(const std::type_info& t) = delete;

    /// Helper method to extract the namespace (removing class name and templates)
    SOFA_ATTRIBUTE_DISABLED__CLASSNAME_INTROSPECTION()
    static std::string decodeNamespaceName(const std::type_info& t) = delete;

    /// Helper method to extract the template name (removing namespaces and class name)
    SOFA_ATTRIBUTE_DISABLED__CLASSNAME_INTROSPECTION()
    static std::string decodeTemplateName(const std::type_info& t) = delete;

    /// Helper method to get the type name
    template<class T>
    SOFA_ATTRIBUTE_DISABLED__CLASSNAME_INTROSPECTION()
    static std::string defaultTypeName(const T* = nullptr) = delete;
};

class SOFA_CORE_API DeprecatedBaseClass : public BaseClass
{
public:
    DeprecatedBaseClass();

    Base* dynamicCast(Base*) const override { return nullptr; }
    bool isInstance(Base*) const override { return false; }

    static BaseClass* GetSingleton();
};


// To specify template classes in C macro parameters, we can't write any commas, hence templates with more than 2 parameters have to use the following macros
#define SOFA_TEMPLATE(Class,P1) Class<P1>
#define SOFA_TEMPLATE2(Class,P1,P2) Class<P1,P2>
#define SOFA_TEMPLATE3(Class,P1,P2,P3) Class<P1,P2,P3>
#define SOFA_TEMPLATE4(Class,P1,P2,P3,P4) Class<P1,P2,P3,P4>

// This macro should now be used at the beginning of all declarations of classes with 1 base class
#define SOFA_CLASS(T,Parent) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, Parent > MyClass; \
    typedef Parent Inherit1; \
    SOFA_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 1 base class
#define SOFA_ABSTRACT_CLASS(T,Parent) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, Parent > MyClass; \
    typedef Parent Inherit1; \
    SOFA_ABSTRACT_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 2 base classes
#define SOFA_CLASS2(T,Parent1,Parent2) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<Parent1,Parent2> > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    SOFA_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 2 base classes
#define SOFA_ABSTRACT_CLASS2(T,Parent1,Parent2) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<Parent1,Parent2> > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    SOFA_ABSTRACT_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 3 base classes
#define SOFA_CLASS3(T,Parent1,Parent2,Parent3) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<Parent1,std::pair<Parent2,Parent3> > > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    typedef Parent3 Inherit3; \
    SOFA_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 3 base classes
#define SOFA_ABSTRACT_CLASS3(T,Parent1,Parent2,Parent3) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<Parent1,std::pair<Parent2,Parent3> > > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    typedef Parent3 Inherit3; \
    SOFA_ABSTRACT_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 4 base classes
#define SOFA_CLASS4(T,Parent1,Parent2,Parent3,Parent4) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<std::pair<Parent1,Parent2>,std::pair<Parent3,Parent4> > > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    typedef Parent3 Inherit3; \
    typedef Parent4 Inherit4; \
    SOFA_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 4 base classes
#define SOFA_ABSTRACT_CLASS4(T,Parent1,Parent2,Parent3,Parent4) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<std::pair<Parent1,Parent2>,std::pair<Parent3,Parent4> > > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    typedef Parent3 Inherit3; \
    typedef Parent4 Inherit4; \
    SOFA_ABSTRACT_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 5 base classes
#define SOFA_CLASS5(T,Parent1,Parent2,Parent3,Parent4,Parent5) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<std::pair<Parent1,Parent2>,std::pair<Parent3,std::pair<Parent4,Parent5> > > > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    typedef Parent3 Inherit3; \
    typedef Parent4 Inherit4; \
    typedef Parent5 Inherit5; \
    SOFA_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 5 base classes
#define SOFA_ABSTRACT_CLASS5(T,Parent1,Parent2,Parent3,Parent4,Parent5) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<std::pair<Parent1,Parent2>,std::pair<Parent3,std::pair<Parent4,Parent5> > > > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    typedef Parent3 Inherit3; \
    typedef Parent4 Inherit4; \
    typedef Parent5 Inherit5; \
    SOFA_ABSTRACT_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 5 base classes
#define SOFA_CLASS6(T,Parent1,Parent2,Parent3,Parent4,Parent5,Parent6) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<std::pair<Parent1,Parent2>,std::pair<std::pair<Parent3,Parent4>,std::pair<Parent5,Parent6> > > > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    typedef Parent3 Inherit3; \
    typedef Parent4 Inherit4; \
    typedef Parent5 Inherit5; \
    typedef Parent6 Inherit6; \
    SOFA_CLASS_DECL

// This macro should now be used at the beginning of all declarations of classes with 5 base classes
#define SOFA_ABSTRACT_CLASS6(T,Parent1,Parent2,Parent3,Parent4,Parent5,Parent6) \
    typedef T MyType;                                               \
    typedef ::sofa::core::objectmodel::TClass< T, std::pair<std::pair<Parent1,Parent2>,std::pair<std::pair<Parent3,Parent4>,std::pair<Parent5,Parent6> > > > MyClass; \
    typedef Parent1 Inherit1; \
    typedef Parent2 Inherit2; \
    typedef Parent3 Inherit3; \
    typedef Parent4 Inherit4; \
    typedef Parent5 Inherit5; \
    typedef Parent6 Inherit6; \
    SOFA_ABSTRACT_CLASS_DECL

// Do not use this macro directly, use SOFA_ABSTRACT_CLASS instead
#define SOFA_ABSTRACT_CLASS_DECL                                        \
    typedef MyType* Ptr;                                                \
    friend class sofa::helper::NameDecoder;                             \
    static std::string GetDefaultTemplateName(){ return sofa::helper::NameDecoder::DefaultTypeTemplateName<MyType>::Get(); } \
    using SPtr = sofa::core::sptr<MyType>;                              \
    static const ::sofa::core::objectmodel::BaseClass* GetClass() { return MyClass::get(); }   \
    virtual const ::sofa::core::objectmodel::BaseClass* getClass() const override \
{ return GetClass(); }                                              \
    static const char* HeaderFileLocation() { return __FILE__; }        \
    template<class SOFA_T> ::sofa::core::objectmodel::BaseData::BaseInitData \
    initData(::sofa::core::objectmodel::Data<SOFA_T>* field, const char* name, const char* help,   \
    ::sofa::core::objectmodel::BaseData::DataFlags dataflags)  \
{                                                                   \
    ::sofa::core::objectmodel::BaseData::BaseInitData res;          \
    this->initData0(field, res, name, help, dataflags);             \
    return res;                                                     \
}                                                                   \
    template<class SOFA_T> ::sofa::core::objectmodel::BaseData::BaseInitData \
    initData(::sofa::core::objectmodel::Data<SOFA_T>* field, const char* name, const char* help,   \
    bool isDisplayed=true, bool isReadOnly=false)              \
{                                                                   \
    ::sofa::core::objectmodel::BaseData::BaseInitData res;          \
    this->initData0(field, res, name, help,                         \
    isDisplayed, isReadOnly);                       \
    return res;                                                     \
}                                                                   \
    template<class SOFA_T> typename ::sofa::core::objectmodel::Data<SOFA_T>::InitData initData(    \
    ::sofa::core::objectmodel::Data<SOFA_T>* field, const SOFA_T& value, const char* name,     \
    const char* help, bool isDisplayed=true, bool isReadOnly=false) \
{                                                                   \
    typename ::sofa::core::objectmodel::Data<SOFA_T>::InitData res; \
    this->initData0(field, res, value, name, help,                  \
    isDisplayed, isReadOnly);                       \
    return res;                                                     \
}                                                                   \
    ::sofa::core::objectmodel::BaseLink::InitLink<MyType>               \
    initLink(const char* name, const char* help)                        \
{                                                                   \
    return ::sofa::core::objectmodel::BaseLink::InitLink<MyType>    \
    (this, name, help);                                             \
}                                                                   \
    using Inherit1::sout;                                               \
    using Inherit1::serr;                                               \
    using Inherit1::sendl

// Do not use this macro directly, use SOFA_CLASS instead
#define SOFA_CLASS_DECL                                        \
    SOFA_ABSTRACT_CLASS_DECL;                                  \
    \
    friend class sofa::core::objectmodel::New<MyType>

template <class Parents>
class TClassParents
{
public:
    static int nb()
    {
        return 1;
    }
    static const BaseClass* get(int i)
    {
        if (i==0)
            return Parents::GetClass();
        else
            return nullptr;
    }
};

template<>
class TClassParents<void>
{
public:
    static int nb()
    {
        return 0;
    }
    static const BaseClass* get(int)
    {
        return nullptr;
    }
};

template<class P1, class P2>
class TClassParents< std::pair<P1,P2> >
{
public:
    static int nb()
    {
        return TClassParents<P1>::nb() + TClassParents<P2>::nb();
    }
    static const BaseClass* get(int i)
    {
        if (i<TClassParents<P1>::nb())
            return TClassParents<P1>::get(i);
        else
            return TClassParents<P2>::get(i-TClassParents<P1>::nb());
    }
};

template <class T, class Parents = void>
class TClass : public BaseClass
{
protected:
    TClass()
    {
        typeName = NameDecoder::getTypeName<T>();
        namespaceName = NameDecoder::getNamespaceName<T>();
        className = NameDecoder::getClassName<T>();
        templateName = NameDecoder::getTemplateName<T>();
        shortName = NameDecoder::getShortName<T>();

        parents.resize(TClassParents<Parents>::nb());
        for (int i=0; i<TClassParents<Parents>::nb(); ++i)
            parents[i] = TClassParents<Parents>::get(i);
    }
    ~TClass() override {}

    Base* dynamicCast(Base* obj) const override
    {
        return dynamic_cast<T*>(obj);
    }

    bool isInstance(Base* obj) const override
    {
        return dynamicCast(obj) != nullptr;
    }

public:

    static const BaseClass* get()
    {
        static TClass<T, Parents> *theClass=new TClass<T, Parents>();
        return theClass;
    }
};

} // namespace objectmodel

} // namespace core

} // namespace sofa



#endif

