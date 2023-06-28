#pragma once

DESCENSUS_API DECLARE_LOG_CATEGORY_EXTERN(LogDes, Log, All);

#define DES_LOG(Type, Format, ...) UE_LOG(LogDes, Type, Format, ##__VA_ARGS__)
#define DES_LOG_CSTR(Type, String) UE_LOG(LogDes, Type, TEXT("%s"), TEXT(String))
#define DES_LOG_INT(Prefix, Value) UE_LOG(LogDes, Warning, TEXT(Prefix  ": %i"), Value)
#define DES_LOG_BOOL(Prefix, Value) UE_LOG(LogDes, Warning, TEXT(Prefix  ": %s"), Value ? TEXT("true") : TEXT("false"))
#define DES_LOG_EXPR(Expr) UE_LOG(LogDes, Warning, TEXT("%s"), *FString::Format(TEXT(#Expr ": {0}"), {(Expr)}))
