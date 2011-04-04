//
// Generated file, do not edit! Created by opp_msgc 4.1 from src/cdn/message/requestCDN.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "requestCDN_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




RequestCDN_Base::RequestCDN_Base(const char *name, int kind) : cPacket(name,kind)
{
    this->cdnId_var = 0;
    this->clientId_var = 0;
    this->segmentId_var = 0;
    this->source_var = 0;
    this->destination_var = 0;
    this->hopCount_var = 0;
}

RequestCDN_Base::RequestCDN_Base(const RequestCDN_Base& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

RequestCDN_Base::~RequestCDN_Base()
{
}

RequestCDN_Base& RequestCDN_Base::operator=(const RequestCDN_Base& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->cdnId_var = other.cdnId_var;
    this->clientId_var = other.clientId_var;
    this->segmentId_var = other.segmentId_var;
    this->source_var = other.source_var;
    this->destination_var = other.destination_var;
    this->hopCount_var = other.hopCount_var;
    return *this;
}

void RequestCDN_Base::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->cdnId_var);
    doPacking(b,this->clientId_var);
    doPacking(b,this->segmentId_var);
    doPacking(b,this->source_var);
    doPacking(b,this->destination_var);
    doPacking(b,this->hopCount_var);
}

void RequestCDN_Base::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->cdnId_var);
    doUnpacking(b,this->clientId_var);
    doUnpacking(b,this->segmentId_var);
    doUnpacking(b,this->source_var);
    doUnpacking(b,this->destination_var);
    doUnpacking(b,this->hopCount_var);
}

int RequestCDN_Base::getCdnId() const
{
    return cdnId_var;
}

void RequestCDN_Base::setCdnId(int cdnId_var)
{
    this->cdnId_var = cdnId_var;
}

int RequestCDN_Base::getClientId() const
{
    return clientId_var;
}

void RequestCDN_Base::setClientId(int clientId_var)
{
    this->clientId_var = clientId_var;
}

int RequestCDN_Base::getSegmentId() const
{
    return segmentId_var;
}

void RequestCDN_Base::setSegmentId(int segmentId_var)
{
    this->segmentId_var = segmentId_var;
}

int RequestCDN_Base::getSource() const
{
    return source_var;
}

void RequestCDN_Base::setSource(int source_var)
{
    this->source_var = source_var;
}

int RequestCDN_Base::getDestination() const
{
    return destination_var;
}

void RequestCDN_Base::setDestination(int destination_var)
{
    this->destination_var = destination_var;
}

int RequestCDN_Base::getHopCount() const
{
    return hopCount_var;
}

void RequestCDN_Base::setHopCount(int hopCount_var)
{
    this->hopCount_var = hopCount_var;
}

class RequestCDNDescriptor : public cClassDescriptor
{
  public:
    RequestCDNDescriptor();
    virtual ~RequestCDNDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(RequestCDNDescriptor);

RequestCDNDescriptor::RequestCDNDescriptor() : cClassDescriptor("RequestCDN", "cPacket")
{
}

RequestCDNDescriptor::~RequestCDNDescriptor()
{
}

bool RequestCDNDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<RequestCDN_Base *>(obj)!=NULL;
}

const char *RequestCDNDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int RequestCDNDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount(object) : 6;
}

unsigned int RequestCDNDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *RequestCDNDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "cdnId",
        "clientId",
        "segmentId",
        "source",
        "destination",
        "hopCount",
    };
    return (field>=0 && field<6) ? fieldNames[field] : NULL;
}

int RequestCDNDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cdnId")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "clientId")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "segmentId")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "source")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "destination")==0) return base+4;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+5;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *RequestCDNDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : NULL;
}

const char *RequestCDNDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int RequestCDNDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    RequestCDN_Base *pp = (RequestCDN_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string RequestCDNDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    RequestCDN_Base *pp = (RequestCDN_Base *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCdnId());
        case 1: return long2string(pp->getClientId());
        case 2: return long2string(pp->getSegmentId());
        case 3: return long2string(pp->getSource());
        case 4: return long2string(pp->getDestination());
        case 5: return long2string(pp->getHopCount());
        default: return "";
    }
}

bool RequestCDNDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    RequestCDN_Base *pp = (RequestCDN_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setCdnId(string2long(value)); return true;
        case 1: pp->setClientId(string2long(value)); return true;
        case 2: pp->setSegmentId(string2long(value)); return true;
        case 3: pp->setSource(string2long(value)); return true;
        case 4: pp->setDestination(string2long(value)); return true;
        case 5: pp->setHopCount(string2long(value)); return true;
        default: return false;
    }
}

const char *RequestCDNDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<6) ? fieldStructNames[field] : NULL;
}

void *RequestCDNDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    RequestCDN_Base *pp = (RequestCDN_Base *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


