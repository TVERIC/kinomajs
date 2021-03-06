/*
 *     Copyright (C) 2010-2016 Marvell International Ltd.
 *     Copyright (C) 2002-2010 Kinoma, Inc.
 *
 *     Licensed under the Apache License, Version 2.0 (the "License");
 *     you may not use this file except in compliance with the License.
 *     You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */
#include "xs6All.h"

static void fx_Object(txMachine* the);
static void fx_Object_prototype___proto__get(txMachine* the);
static void fx_Object_prototype___proto__set(txMachine* the);
static void fx_Object_prototype_hasOwnProperty(txMachine* the);
static void fx_Object_prototype_isPrototypeOf(txMachine* the);
static void fx_Object_prototype_propertyIsEnumerable(txMachine* the);
static void fx_Object_prototype_propertyIsScriptable(txMachine* the);
static void fx_Object_prototype_toLocaleString(txMachine* the);
static void fx_Object_prototype_toPrimitive(txMachine* the);
static void fx_Object_prototype_toString(txMachine* the);
static void fx_Object_prototype_valueOf(txMachine* the);

static void fx_Object_assign(txMachine* the);
static void fx_Object_create(txMachine* the);
static void fx_Object_defineProperties(txMachine* the);
static void fx_Object_defineProperty(txMachine* the);
static void fx_Object_freeze(txMachine* the);
static void fx_Object_getOwnPropertyDescriptor(txMachine* the);
static void fx_Object_getOwnPropertyNames(txMachine* the);
static void fx_Object_getOwnPropertySymbols(txMachine* the);
static void fx_Object_getPrototypeOf(txMachine* the);
static void fx_Object_is(txMachine* the);
static void fx_Object_isExtensible(txMachine* the);
static void fx_Object_isFrozen(txMachine* the);
static void fx_Object_isSealed(txMachine* the);
static void fx_Object_keys(txMachine* the);
static void fx_Object_preventExtensions(txMachine* the);
static void fx_Object_seal(txMachine* the);
static void fx_Object_setPrototypeOf(txMachine* the);

static void fxAssignPropertyStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property);
static void fxDefinePropertyStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property);
static void fxFreezePropertyStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property);
static void fxIsPropertyFrozenStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property);
static void fxIsPropertySealedStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property);
static void fxSealPropertyStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property);

void fxBuildObject(txMachine* the)
{
    static const txHostFunctionBuilder gx_Object_prototype_builders[] = {
		{ fx_Object_prototype_hasOwnProperty, 1, _hasOwnProperty },
		{ fx_Object_prototype_isPrototypeOf, 1, _isPrototypeOf },
		{ fx_Object_prototype_propertyIsEnumerable, 1, _propertyIsEnumerable },
		{ fx_Object_prototype_propertyIsScriptable, 1, _propertyIsScriptable },
		{ fx_Object_prototype_toLocaleString, 0, _toLocaleString },
		{ fx_Object_prototype_toPrimitive, 1, _Symbol_toPrimitive },
		{ fx_Object_prototype_toString, 0, _toString },
		{ fx_Object_prototype_valueOf, 0, _valueOf },
		{ C_NULL, 0, 0 },
    };
    static const txHostFunctionBuilder gx_Object_builders[] = {
		{ fx_Object_assign, 2, _assign },
		{ fx_Object_create, 2, _create },
		{ fx_Object_defineProperties, 2, _defineProperties },
		{ fx_Object_defineProperty, 3, _defineProperty },
		{ fx_Object_freeze, 1, _freeze },
		{ fx_Object_getOwnPropertyDescriptor, 2, _getOwnPropertyDescriptor },
		{ fx_Object_getOwnPropertyNames, 1, _getOwnPropertyNames },
		{ fx_Object_getOwnPropertySymbols, 1, _getOwnPropertySymbols },
		{ fx_Object_getPrototypeOf, 1, _getPrototypeOf },
		{ fx_Object_is, 2, _is },
		{ fx_Object_isExtensible, 1, _isExtensible },
		{ fx_Object_isFrozen, 1, _isFrozen },
		{ fx_Object_isSealed, 1, _isSealed },
		{ fx_Object_keys, 1, _keys },
		{ fx_Object_preventExtensions, 1, _preventExtensions },
		{ fx_Object_seal, 1, _seal },
		{ fx_Object_setPrototypeOf, 2, _setPrototypeOf },
		{ C_NULL, 0, 0 },
    };
    const txHostFunctionBuilder* builder;
	txSlot* slot;
	mxPush(mxObjectPrototype);
	slot = fxLastProperty(the, the->stack->value.reference);
	slot = fxNextHostAccessorProperty(the, slot, fx_Object_prototype___proto__get, fx_Object_prototype___proto__set, mxID(___proto__), XS_DONT_ENUM_FLAG | XS_DONT_SET_FLAG);
	for (builder = gx_Object_prototype_builders; builder->callback; builder++)
		slot = fxNextHostFunctionProperty(the, slot, builder->callback, builder->length, mxID(builder->id), XS_DONT_ENUM_FLAG);
	slot = fxLastProperty(the, fxNewHostConstructorGlobal(the, fx_Object, 1, mxID(_Object), XS_DONT_ENUM_FLAG));
	for (builder = gx_Object_builders; builder->callback; builder++)
		slot = fxNextHostFunctionProperty(the, slot, builder->callback, builder->length, mxID(builder->id), XS_DONT_ENUM_FLAG);
	the->stack++;
}

txSlot* fxNewObjectInstance(txMachine* the)
{
	txSlot* instance;
	instance = fxNewSlot(the);
	instance->kind = XS_INSTANCE_KIND;
	instance->value.instance.garbage = C_NULL;
	instance->value.instance.prototype = the->stack->value.reference;
	the->stack->value.reference = instance;
	the->stack->kind = XS_REFERENCE_KIND;
	return instance;
}

void fx_Object(txMachine* the)
{
	if ((mxArgc == 0) || (mxArgv(0)->kind == XS_UNDEFINED_KIND) || (mxArgv(0)->kind == XS_NULL_KIND)) {
		if (mxTarget->kind == XS_UNDEFINED_KIND) {
			mxPush(mxObjectPrototype);
			fxNewObjectInstance(the);
			mxPullSlot(mxResult);
		}
	}
	else {
		*mxResult = *mxArgv(0);
		fxToInstance(the, mxResult);
	}
}

void fx_Object_prototype___proto__get(txMachine* the)
{
	if ((mxThis->kind == XS_UNDEFINED_KIND) || (mxThis->kind == XS_NULL_KIND))
		mxTypeError("invalid this");
	fxGetInstancePrototype(the, fxToInstance(the, mxThis), mxResult);
}

void fx_Object_prototype___proto__set(txMachine* the)
{
	txSlot* instance;
	if ((mxThis->kind == XS_UNDEFINED_KIND) || (mxThis->kind == XS_NULL_KIND))
		mxTypeError("invalid this");
	if ((mxArgc < 1) || ((mxArgv(0)->kind != XS_NULL_KIND) && (mxArgv(0)->kind != XS_REFERENCE_KIND)))
		mxTypeError("invalid prototype");
	if (mxThis->kind == XS_REFERENCE_KIND) {
		instance = mxThis->value.reference;
		if (!fxSetInstancePrototype(the, instance, mxArgv(0)))
			mxTypeError("invalid prototype");
	}
}

void fx_Object_prototype_hasOwnProperty(txMachine* the)
{
	txSlot* instance;
	txSlot* at;
	if ((mxThis->kind == XS_UNDEFINED_KIND) || (mxThis->kind == XS_NULL_KIND))
		mxTypeError("invalid this");
	if (mxArgc < 1)
		mxTypeError("invalid key");
	instance = fxToInstance(the, mxThis);
	at = fxAt(the, mxArgv(0));
	mxPushUndefined();
	mxResult->value.boolean = fxGetInstanceOwnProperty(the, instance, at->value.at.id, at->value.at.index, the->stack);
	mxResult->kind = XS_BOOLEAN_KIND;
	mxPop();
}

void fx_Object_prototype_isPrototypeOf(txMachine* the)
{
	if ((mxThis->kind == XS_UNDEFINED_KIND) || (mxThis->kind == XS_NULL_KIND))
		mxTypeError("invalid this");
	mxResult->kind = XS_BOOLEAN_KIND;
	mxResult->value.boolean = 0;
	if (mxArgc > 0) {
		txSlot* slot = mxArgv(0);
		if (slot->kind == XS_REFERENCE_KIND) {
			txSlot* prototype = fxToInstance(the, mxThis);
			txSlot* instance = fxGetParent(the, slot->value.reference);
			while (instance) {
				if (prototype == instance) {
					mxResult->value.boolean = 1;
					break;
				}
				instance = fxGetParent(the, instance);
			}
		}
	}
}

void fx_Object_prototype_propertyIsEnumerable(txMachine* the)
{
	txSlot* instance;
	txSlot* at;
	if ((mxThis->kind == XS_UNDEFINED_KIND) || (mxThis->kind == XS_NULL_KIND))
		mxTypeError("invalid this");
	if (mxArgc < 1)
		mxTypeError("invalid key");
	mxResult->value.boolean = 0;
	instance = fxToInstance(the, mxThis);
	at = fxAt(the, mxArgv(0));
	mxPushUndefined();
	mxResult->value.boolean = fxGetInstanceOwnProperty(the, instance, at->value.at.id, at->value.at.index, the->stack) && ((the->stack->flag & XS_DONT_ENUM_FLAG) == 0);
	mxResult->kind = XS_BOOLEAN_KIND;
	mxPop();
}

void fx_Object_prototype_propertyIsScriptable(txMachine* the)
{
	mxResult->kind = XS_BOOLEAN_KIND;
	mxResult->value.boolean = 1;
}

void fx_Object_prototype_toLocaleString(txMachine* the)
{
	mxPushInteger(0);
	mxPushSlot(mxThis);
	mxPushSlot(mxThis);
	fxGetID(the, mxID(_toString));
	fxCall(the);
	mxPullSlot(mxResult);
}

void fx_Object_prototype_toPrimitive(txMachine* the)
{
	if (mxThis->kind == XS_REFERENCE_KIND) {
		txInteger hint = XS_NO_HINT;
		txInteger ids[2], i;
		if (mxArgc > 0) {
			txSlot* slot = mxArgv(0);
			if ((slot->kind == XS_STRING_KIND) || (slot->kind == XS_STRING_X_KIND)) {
				if (!c_strcmp(slot->value.string, "default"))
					hint = XS_NUMBER_HINT;
				else if (!c_strcmp(slot->value.string, "number"))
					hint = XS_NUMBER_HINT;
				else if (!c_strcmp(slot->value.string, "string"))
					hint = XS_STRING_HINT;
			}
		}
		if (hint == XS_STRING_HINT) {
		 	ids[0] = mxID(_toString);
		 	ids[1] = mxID(_valueOf);
		}
		else if (hint == XS_NUMBER_HINT) {
		 	ids[0] = mxID(_valueOf);
		 	ids[1] = mxID(_toString);
		}
 		else
     		mxTypeError("invalid hint");
		for (i = 0; i < 2; i++) {
			mxPushInteger(0);
			mxPushSlot(mxThis);
			mxPushSlot(mxThis);
			fxGetID(the, ids[i]);
			if (mxIsReference(the->stack) && mxIsFunction(the->stack->value.reference)) {
				fxCall(the);
				if (mxIsReference(the->stack))
					the->stack++;
				else {
					mxResult->kind = the->stack->kind;
					mxResult->value = the->stack->value;
					the->stack++;
					return;
      			}
			}
			else
				the->stack++;
		}
		if (hint == XS_STRING_HINT)
            mxTypeError("cannot coerce object to string");
        else
            mxTypeError("cannot coerce object to number");
	}
	else {
		mxResult->kind = mxThis->kind;
		mxResult->value = mxThis->value;
	}
}

void fx_Object_prototype_toString(txMachine* the)
{
	txString tag = C_NULL;
	txSlot* instance;
	txSlot* slot;
	switch (mxThis->kind) {
	case XS_UNDEFINED_KIND:
		tag = "Undefined";
		break;
	case XS_NULL_KIND:
		tag = "Null";
		break;
	case XS_BOOLEAN_KIND:
		tag = "Boolean";
		break;
	case XS_INTEGER_KIND:
	case XS_NUMBER_KIND:
		tag = "Number";
		break;
	case XS_STRING_KIND:
	case XS_STRING_X_KIND:
		tag = "String";
		break;
	case XS_SYMBOL_KIND:
		tag = "Symbol";
		break;
	case XS_REFERENCE_KIND:
		instance = mxThis->value.reference;
		if (((slot = instance->next)) && (slot->flag & XS_INTERNAL_FLAG)) {
			switch (slot->kind) {
			case XS_ARRAY_KIND:
				tag = "Array";
				break;
			case XS_BOOLEAN_KIND:
				tag = "Boolean";
				break;
			case XS_CALLBACK_KIND:
			case XS_CODE_KIND:
			case XS_CODE_X_KIND:
				tag = "Function";
				break;
			case XS_DATE_KIND:
				tag = "Date";
				break;
			case XS_ERROR_KIND:
				tag = "Error";
				break;
			case XS_GLOBAL_KIND:
				tag = "global";
				break;
			case XS_NUMBER_KIND:
				tag = "Number";
				break;
			case XS_ARGUMENTS_SLOPPY_KIND:
			case XS_ARGUMENTS_STRICT_KIND:
				tag = "Arguments";
				break;
			case XS_REGEXP_KIND:
				tag = "RegExp";
				break;
			case XS_STRING_KIND:
			case XS_STRING_X_KIND:
				tag = "String";
				break;
            default:
                tag = "Object";
                break;
            }
		}
        else {
            tag = "Object";
        }
		break;
	default:
		tag = "Object";
		break;
	}
	fxCopyStringC(the, mxResult, "[object ");
	if (mxThis->kind == XS_REFERENCE_KIND) {
		mxPushSlot(mxThis);
		fxGetID(the, mxID(_Symbol_toStringTag));
		if ((the->stack->kind == XS_STRING_KIND) || (the->stack->kind == XS_STRING_X_KIND))
			fxConcatString(the, mxResult, the->stack);
		else
			fxConcatStringC(the, mxResult, tag);
	}
	else
		fxConcatStringC(the, mxResult, tag);
	fxConcatStringC(the, mxResult, "]");
}

void fx_Object_prototype_valueOf(txMachine* the)
{
	fxToInstance(the, mxThis);
	*mxResult = *mxThis;
}

void fx_Object_assign(txMachine* the)
{
	txSlot* target;
	txInteger c, i;
	txSlot* source;
	if ((mxArgc < 1) || (mxArgv(0)->kind == XS_UNDEFINED_KIND) || (mxArgv(0)->kind == XS_NULL_KIND))
		mxTypeError("invalid target");
	fxToInstance(the, mxArgv(0));
	target = mxArgv(0);
	c = mxArgc;
	for (i = 1; i < c; i++) {
		if ((mxArgv(i)->kind == XS_UNDEFINED_KIND) || (mxArgv(i)->kind == XS_NULL_KIND))
			continue;
		source = fxToInstance(the, mxArgv(i));
		fxEachInstanceProperty(the, source, XS_EACH_ENUMERABLE_FLAG | XS_EACH_STRING_FLAG | XS_EACH_SYMBOL_FLAG | XS_STEP_GET_OWN_FLAG | XS_STEP_GET_FLAG, fxAssignPropertyStep, target, source);
	}
	*mxResult = *target;
}

void fx_Object_create(txMachine* the)
{
	txSlot* instance;
	txSlot* properties;
	if ((mxArgc < 1) || ((mxArgv(0)->kind != XS_NULL_KIND) && (mxArgv(0)->kind != XS_REFERENCE_KIND)))
		mxTypeError("invalid prototype");
	if (mxArgv(0)->kind == XS_NULL_KIND)
		fxNewInstance(the);
	else {
		mxPushSlot(mxArgv(0));
		fxNewInstanceOf(the);
	}
	mxPullSlot(mxResult);
	instance = fxGetInstance(the, mxResult);
	if ((mxArgc > 1) && (mxArgv(1)->kind != XS_UNDEFINED_KIND)) {
		properties = fxToInstance(the, mxArgv(1));
		fxEachInstanceProperty(the, properties, XS_EACH_ENUMERABLE_FLAG | XS_EACH_STRING_FLAG | XS_EACH_SYMBOL_FLAG | XS_STEP_GET_OWN_FLAG | XS_STEP_GET_FLAG, fxDefinePropertyStep, instance, properties);
	}
}

void fx_Object_defineProperties(txMachine* the)
{
	txSlot* instance;
	txSlot* properties;
	if ((mxArgc < 1) || (mxArgv(0)->kind != XS_REFERENCE_KIND))
		mxTypeError("invalid object");
	if ((mxArgc < 2) || (mxArgv(1)->kind == XS_UNDEFINED_KIND))
		mxTypeError("invalid properties");
	instance = fxGetInstance(the, mxArgv(0));
	properties = fxToInstance(the, mxArgv(1));
	fxEachInstanceProperty(the, properties, XS_EACH_ENUMERABLE_FLAG | XS_EACH_STRING_FLAG | XS_EACH_SYMBOL_FLAG | XS_STEP_GET_OWN_FLAG | XS_STEP_GET_FLAG, fxDefinePropertyStep, instance, properties);
	*mxResult = *mxArgv(0);
}

void fx_Object_defineProperty(txMachine* the)
{
	txSlot* at;
	txFlag mask;
	if ((mxArgc < 1) || (mxArgv(0)->kind != XS_REFERENCE_KIND))
		mxTypeError("invalid object");
	if (mxArgc < 2)
		mxTypeError("invalid key");
	at = fxAt(the, mxArgv(1));
	if ((mxArgc < 3) || (mxArgv(2)->kind != XS_REFERENCE_KIND))
		mxTypeError("invalid descriptor");
	mask = fxDescriptorToSlot(the, mxArgv(2));
	if(!fxDefineInstanceProperty(the, mxArgv(0)->value.reference, at->value.at.id, at->value.at.index, mxArgv(2), mask))
		mxTypeError("invalid descriptor");
	*mxResult = *mxArgv(0);
}

void fx_Object_freeze(txMachine* the)
{
	if (mxArgc > 0) {
		txSlot* slot = mxArgv(0);
		if (slot->kind == XS_REFERENCE_KIND) {
			slot = slot->value.reference;
			fxPreventInstanceExtensions(the, slot);
			fxEachInstanceProperty(the, slot, XS_EACH_STRING_FLAG | XS_EACH_SYMBOL_FLAG | XS_STEP_GET_OWN_FLAG | XS_STEP_DEFINE_FLAG, fxFreezePropertyStep, mxResult, slot);
		}
		*mxResult = *mxArgv(0);
	}
}

void fx_Object_getOwnPropertyDescriptor(txMachine* the)
{
	txSlot* instance;
	txSlot* at;
	if ((mxArgc < 1) || (mxArgv(0)->kind == XS_UNDEFINED_KIND) || (mxArgv(0)->kind == XS_NULL_KIND))
		mxTypeError("invalid object");
	instance = fxToInstance(the, mxArgv(0));
	if (mxArgc < 2)
		mxPushUndefined();
	else
		mxPushSlot(mxArgv(1));
	at = fxAt(the, the->stack);
	mxPushUndefined();
	if (fxGetInstanceOwnProperty(the, instance, at->value.at.id, at->value.at.index, the->stack)) {
		fxDescribeProperty(the, the->stack, XS_GET_ONLY);
		mxPullSlot(mxResult);
	}
	mxPop();
	mxPop();
}

void fx_Object_getOwnPropertyNames(txMachine* the)
{
	if ((mxArgc < 1) || (mxArgv(0)->kind == XS_UNDEFINED_KIND) || (mxArgv(0)->kind == XS_NULL_KIND))
		mxTypeError("invalid object");
	fxEnumProperties(the, fxToInstance(the, mxArgv(0)), XS_EACH_STRING_FLAG);
}

void fx_Object_getOwnPropertySymbols(txMachine* the)
{
	if ((mxArgc < 1) || (mxArgv(0)->kind == XS_UNDEFINED_KIND) || (mxArgv(0)->kind == XS_NULL_KIND))
		mxTypeError("invalid object");
	fxEnumProperties(the, fxToInstance(the, mxArgv(0)), XS_EACH_SYMBOL_FLAG);
}

void fx_Object_getPrototypeOf(txMachine* the)
{
	if ((mxArgc < 1) || (mxArgv(0)->kind == XS_UNDEFINED_KIND) || (mxArgv(0)->kind == XS_NULL_KIND))
		mxTypeError("invalid object");
	fxGetInstancePrototype(the, fxToInstance(the, mxArgv(0)), mxResult);
}

void fx_Object_is(txMachine* the)
{
	if (mxArgc > 0)
		mxPushSlot(mxArgv(0));
	else
		mxPushUndefined();
	if (mxArgc > 1)
		mxPushSlot(mxArgv(1));
	else
		mxPushUndefined();
	mxResult->value.boolean = fxIsSameValue(the, the->stack + 1, the->stack, 0);
	mxResult->kind = XS_BOOLEAN_KIND;
	the->stack += 2;
}

void fx_Object_isExtensible(txMachine* the)
{
	mxResult->kind = XS_BOOLEAN_KIND;
	mxResult->value.boolean = 0;
	if (mxArgc > 0) {
		txSlot* slot = mxArgv(0);
		if (slot->kind == XS_REFERENCE_KIND) {
			slot = slot->value.reference;
			mxResult->value.boolean = fxIsInstanceExtensible(the, slot);
		}
	}
}

void fx_Object_isFrozen(txMachine* the)
{
	mxResult->kind = XS_BOOLEAN_KIND;
	mxResult->value.boolean = 1;
	if (mxArgc > 0) {
		txSlot* slot = mxArgv(0);
		if (slot->kind == XS_REFERENCE_KIND) {
			slot = slot->value.reference;
            mxResult->value.boolean = fxIsInstanceExtensible(the, slot) ? 0 : 1;
			if (mxResult->value.boolean) {
				fxEachInstanceProperty(the, slot, XS_EACH_STRING_FLAG | XS_EACH_SYMBOL_FLAG | XS_STEP_GET_OWN_FLAG, fxIsPropertyFrozenStep, mxResult, slot);
			}
		}
	}
}

void fx_Object_isSealed(txMachine* the)
{
	mxResult->kind = XS_BOOLEAN_KIND;
	mxResult->value.boolean = 1;
	if (mxArgc > 0) {
		txSlot* slot = mxArgv(0);
		if (slot->kind == XS_REFERENCE_KIND) {
			slot = slot->value.reference;
			mxResult->value.boolean = fxIsInstanceExtensible(the, slot) ? 0 : 1;
			if (mxResult->value.boolean)
				fxEachInstanceProperty(the, slot, XS_EACH_STRING_FLAG | XS_EACH_SYMBOL_FLAG | XS_STEP_GET_OWN_FLAG, fxIsPropertySealedStep, mxResult, slot);
		}
	}
}

void fx_Object_keys(txMachine* the)
{
	if ((mxArgc < 1) || (mxArgv(0)->kind == XS_UNDEFINED_KIND) || (mxArgv(0)->kind == XS_NULL_KIND))
		mxTypeError("invalid object");
	fxEnumProperties(the, fxToInstance(the, mxArgv(0)), XS_EACH_ENUMERABLE_FLAG | XS_EACH_STRING_FLAG);
}

void fx_Object_preventExtensions(txMachine* the)
{
	if (mxArgc > 0) {
		txSlot* slot = mxArgv(0);
		if (slot->kind == XS_REFERENCE_KIND) {
			slot = slot->value.reference;
			fxPreventInstanceExtensions(the, slot);
		}
		*mxResult = *mxArgv(0);
	}
}

void fx_Object_seal(txMachine* the)
{
	if (mxArgc > 0) {
		txSlot* slot = mxArgv(0);
		if (slot->kind == XS_REFERENCE_KIND) {
			slot = slot->value.reference;
			fxPreventInstanceExtensions(the, slot);
			fxEachInstanceProperty(the, slot, XS_EACH_STRING_FLAG | XS_EACH_SYMBOL_FLAG | XS_STEP_GET_OWN_FLAG | XS_STEP_DEFINE_FLAG, fxSealPropertyStep, mxResult, slot);
		}
		*mxResult = *mxArgv(0);
	}
}

void fx_Object_setPrototypeOf(txMachine* the)
{
	txSlot* instance;
	if ((mxArgc < 1) || (mxArgv(0)->kind == XS_UNDEFINED_KIND) || (mxArgv(0)->kind == XS_NULL_KIND))
		mxTypeError("invalid object");
	if ((mxArgc < 2) || ((mxArgv(1)->kind != XS_NULL_KIND) && (mxArgv(1)->kind != XS_REFERENCE_KIND)))
		mxTypeError("invalid prototype");
	if (mxArgv(0)->kind == XS_REFERENCE_KIND) {
		instance = mxArgv(0)->value.reference;
		if (!fxSetInstancePrototype(the, instance, mxArgv(1)))
			mxTypeError("invalid prototype");
	}
	*mxResult = *mxArgv(0);
}

void fxAssignPropertyStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property)
{
	mxPushSlot(property);
	mxPushSlot(context);
	fxSetAll(the, id, index);
	mxPop();
}

void fxDefinePropertyStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property)
{
	txFlag mask;
	mxPushSlot(property);
	mask = fxDescriptorToSlot(the, the->stack);
	if (!fxDefineInstanceProperty(the, context, id, index, the->stack, mask))
		mxTypeError("property is not configurable");
	mxPop();
}

void fxFreezePropertyStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property)
{
	if (property->kind != XS_ACCESSOR_KIND) 
		property->flag |= XS_DONT_SET_FLAG;
	property->flag |= XS_DONT_DELETE_FLAG;
}

void fxIsPropertyFrozenStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property)
{
	if (property->kind != XS_ACCESSOR_KIND) 
		if (!(property->flag & XS_DONT_SET_FLAG))
			context->value.boolean = 0;
	if (!(property->flag & XS_DONT_DELETE_FLAG))
		context->value.boolean = 0;
}

void fxIsPropertySealedStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property)
{
	if (!(property->flag & XS_DONT_DELETE_FLAG))
		context->value.boolean = 0;
}

void fxSealPropertyStep(txMachine* the, txSlot* context, txID id, txIndex index, txSlot* property)
{
	property->flag |= XS_DONT_DELETE_FLAG;
}

