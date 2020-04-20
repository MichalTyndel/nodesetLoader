#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NODECLASS_COUNT 7
typedef enum
{
    NODECLASS_OBJECT = 0,
    NODECLASS_OBJECTTYPE = 1,
    NODECLASS_VARIABLE = 2,
    NODECLASS_DATATYPE = 3,
    NODECLASS_METHOD = 4,
    NODECLASS_REFERENCETYPE = 5,
    NODECLASS_VARIABLETYPE = 6
    // TODO: eventtype missing
} TNodeClass;

typedef struct
{
    int nsIdx;
    char *id;
    char *idString;
} TNodeId;

typedef struct
{
    uint16_t nsIdx;
    char *name;
} TBrowseName;

struct Reference;
typedef struct Reference Reference;

struct Reference
{
    bool isForward;
    TNodeId refType;
    TNodeId target;
    Reference *next;
};

#define UA_NODE_ATTRIBUTES                                                     \
    TNodeClass nodeClass;                                                      \
    TNodeId id;                                                                \
    TBrowseName browseName;                                                    \
    char *displayName;                                                         \
    char *description;                                                         \
    char *writeMask;                                                           \
    Reference *hierachicalRefs;                                                \
    Reference *nonHierachicalRefs;

struct TNode
{
    UA_NODE_ATTRIBUTES
};
typedef struct TNode TNode;

typedef struct
{
    UA_NODE_ATTRIBUTES
    TNodeId parentNodeId;
    char *eventNotifier;
} TObjectNode;

typedef struct
{
    UA_NODE_ATTRIBUTES
    char *isAbstract;
} TObjectTypeNode;

typedef struct
{
    UA_NODE_ATTRIBUTES
    char *isAbstract;
    TNodeId datatype;
    char *arrayDimensions;
    char *valueRank;
} TVariableTypeNode;

/* Value Handling */
struct Value;
typedef struct Value Value;

typedef struct
{
    UA_NODE_ATTRIBUTES
    TNodeId parentNodeId;
    TNodeId datatype;
    char *arrayDimensions;
    char *valueRank;
    char *accessLevel;
    char *userAccessLevel;
    Value *value;
} TVariableNode;

typedef struct TDataTypeNode
{
    UA_NODE_ATTRIBUTES
} TDataTypeNode;

typedef struct
{
    UA_NODE_ATTRIBUTES
    TNodeId parentNodeId;
    char *executable;
    char *userExecutable;
} TMethodNode;

typedef struct
{
    UA_NODE_ATTRIBUTES
    char *symmetric;
} TReferenceTypeNode;

typedef void (*addNodeCb)(void *userContext, const TNode *);
typedef int (*addNamespaceCb)(void *userContext, const char *);

typedef Value *(*newValueCb)(const TNode *node);
typedef void (*startValueCb)(Value *val, const char *localname);
typedef void (*endValueCb)(Value *val, const char *localname, char *value);
typedef void (*finishValueCb)(Value *val);
typedef void (*deleteValueCb)(Value **val);

struct Extension;
typedef struct Extension Extension;

typedef Extension *(*newExtensionCb)(const TNode *);
typedef void (*startExtensionCb)(Extension *ext, const char *localname);
typedef void (*endExtensionCb)(Extension *val, const char *localname,
                               char *value);
typedef void (*finishExtensionCb)(Extension *val);

typedef struct
{
    void *userData;
    newValueCb newValue;
    startValueCb start;
    endValueCb end;
    finishValueCb finish;
    deleteValueCb deleteValue;
} ValueInterface;

typedef struct
{
    void *userData;
    newExtensionCb newExtension;
    startExtensionCb start;
    endExtensionCb end;
    finishExtensionCb finish;
} ExtensionInterface;

typedef struct
{
    void *userContext;
    const char *file;
    addNamespaceCb addNamespace;
    addNodeCb callback;    
    ValueInterface *valueHandling;
    ExtensionInterface *extensionHandling;
} FileContext;

bool loadFile(const FileContext *fileContext);

#ifdef __cplusplus
}
#endif