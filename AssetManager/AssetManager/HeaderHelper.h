#pragma once
\
#define MakeGetter(Variable, Nickname)																		\
public:																								\
	inline const decltype(Variable)& Get##Nickname() const { return Variable; };							

#define MakePointerGetter(Variable, Nickname)																	\
public:																								\
	inline decltype(Variable)* GetPointer##Nickname() { return &Variable; };							

#define MakeSmartPtrGetter(Variable, Nickname)																\
public:																								\
	inline const auto Get##Nickname() const { return Variable.get(); };				

#define MakeComPtrGetter(Variable, Nickname)																	\
public:																								\
	inline const auto Get##Nickname() const { return Variable.Get(); };				
