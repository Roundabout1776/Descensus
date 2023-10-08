#pragma once

#define FIND_DEFAULT_OBJECT(FieldName, ClassName, Path) \
	static ConstructorHelpers::FObjectFinder<UInputAction> Default##FieldName( \
		TEXT(Path)); \
	if (Default##FieldName.Succeeded()) \
	{ \
		FieldName = Default##FieldName.Object; \
	}
