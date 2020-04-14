template <class Derived, class Base>
class ScalarTypeInfo : public Base;

template <class Derived, class Base>
class SingleScalarTypeInfo : public ScalarTypeInfo<Derived, Base>;

template <class Derived, class Base>
class SingleScalarTypeInfoWithTypeLayout : public SingleScalarTypeInfo<Derived, Base>;
// : public ScalarTypeInfo<Derived, Base>
// : public Base

template <class Derived, class Base>
class PODSingleScalarTypeInfo : public SingleScalarTypeInfo<Derived, Base>;
// : public ScalarTypeInfo<Derived, Base>
// : public Base

class TypeInfo;

class FixedTypeInfo : public TypeInfo;

class LoadableTypeInfo : public FixedTypeInfo;

class ReferenceTypeInfo : public LoadableTypeInfo;

template <class Impl>
class HeapTypeInfo : public SingleScalarTypeInfoWithTypeLayout<Impl, ReferenceTypeInfo>;

class ClassTypeInfo : public HeapTypeInfo<ClassTypeInfo>;
// : public SingleScalarTypeInfoWithTypeLayout<ClassTypeInfo, ReferenceTypeInfo>
// : public SingleScalarTypeInfo<ClassTypeInfo, ReferenceTypeInfo>
// : public ScalarTypeInfo<ClassTypeInfo, ReferenceTypeInfo>
// : public ReferenceTypeInfo
// : public LoadableTypeInfo
// : public FixedTypeInfo
// : public TypeInfo

template <class Impl, class Base, class FieldImpl_,
          bool IsLoadable = std::is_base_of<LoadableTypeInfo, Base>::value>
class RecordTypeInfoImpl : public Base,
private llvm::TrailingObjects<Impl, FieldImpl_>;

template <class Impl, class Base, class FieldImpl_,
          bool IsFixedSize = std::is_base_of<FixedTypeInfo, Base>::value,
          bool IsLoadable = std::is_base_of<LoadableTypeInfo, Base>::value>
class RecordTypeInfo;

template <class Impl, class Base, class FieldImpl>
class RecordTypeInfo<Impl, Base, FieldImpl,
                     /*IsFixedSize*/ false, /*IsLoadable*/ false>
: public RecordTypeInfoImpl<Impl, Base, FieldImpl>;
// : public Base

template <class Impl, class Base, class FieldImpl>
class RecordTypeInfo<Impl, Base, FieldImpl,
                     /*IsFixedSize*/ true, /*IsLoadable*/ false>
: public RecordTypeInfoImpl<Impl, Base, FieldImpl>;
// : public Base

template <class Impl, class Base, class FieldImpl>
class RecordTypeInfo<Impl, Base, FieldImpl,
                     /*IsFixedSize*/ true, /*IsLoadable*/ true>
    : public RecordTypeInfo<Impl, Base, FieldImpl, true, false>
// : public RecordTypeInfoImpl<Impl, Base, FieldImpl>
// : public Base

template <class Impl, class Base, class FieldInfoType = StructFieldInfo>
class StructTypeInfoBase
: public RecordTypeInfo<Impl, Base, FieldInfoType>;

class LoadableStructTypeInfo final
: public StructTypeInfoBase<LoadableStructTypeInfo, LoadableTypeInfo>;
// : public RecordTypeInfo<LoadableStructTypeInfo, LoadableTypeInfo, StructFieldInfo, /*IsFixedSize=*/true, /*IsLoadable=*/true>
// : public RecordTypeInfo<LoadableStructTypeInfo, LoadableTypeInfo, StructFieldInfo, /*IsFixedSize=*/true, /*IsLoadable=*/false>
// : public RecordTypeInfoImpl<LoadableStructTypeInfo, LoadableTypeInfo, StructFieldInfo, /*IsLoadable=*/true>
// : public LoadableTypeInfo
// : public FixedTypeInfo
// : public TypeInfo

template <class Derived, class Base>
class IndirectTypeInfo : public Base;

class FixedStructTypeInfo final
: public StructTypeInfoBase<FixedStructTypeInfo, IndirectTypeInfo<FixedStructTypeInfo, FixedTypeInfo>>;
// : public RecordTypeInfo<FixedStructTypeInfo, IndirectTypeInfo<FixedStructTypeInfo, FixedTypeInfo>, StructFieldInfo, /*IsFixedSize=*/true, /*IsLoadable=*/false>
// : public RecordTypeInfoImpl<LoadableStructTypeInfo, LoadableTypeInfo, StructFieldInfo, /*IsLoadable=*/false>
// : public IndirectTypeInfo<FixedStructTypeInfo, FixedTypeInfo>
// : public FixedTypeInfo
// : public TypeInfo

template <class Impl>
class WitnessSizedTypeInfo : public IndirectTypeInfo<Impl, TypeInfo>;
// : public TypeInfo

class NonFixedStructTypeInfo final
: public StructTypeInfoBase<NonFixedStructTypeInfo, WitnessSizedTypeInfo<NonFixedStructTypeInfo>>;
// : public RecordTypeInfo<NonFixedStructTypeInfo, WitnessSizedTypeInfo<NonFixedStructTypeInfo>, StructFieldInfo, /*IsFixedSize=*/false, /*IsLoadable=*/false>
// : public RecordTypeInfoImpl<NonFixedStructTypeInfo, WintessSizedTypeInfo<NonFixedStructTypeInfo>, StructFieldInfo, /*IsLoadable=*/false>
// : public WitnessSizedTypeInfo<NonFixedStructTypeInfo>
// : public IndirectTypeInfo<NonFixedStructTypeInfo, TypeInfo>
// : public TypeInfo

template <class Impl>
class ResilientTypeInfo : public WitnessSizedTypeInfo<Impl>;
// : public IndirectTypeInfo<Impl, TypeInfo>
// : public TypeInfo

class ResilientStructTypeInfo : public ResilientTypeInfo<ResilientStructTypeInfo>;
// : public ResilentTypeInfo : public WitnessSizedTypeInfo<ResilientStructTypeInfo>
// : public IndirectTypeInfo<ResilientStructTypeInfo, TypeInfo>
// : public TypeInfo

class OpaqueArchetypeTypeInfo : public ResilientTypeInfo<OpaqueArchetypeTypeInfo>;
// : public ResilentTypeInfo : public WitnessSizedTypeInfo<OpaqueArchetypeTypeInfo>
// : public IndirectTypeInfo<OpaqueArchetypeTypeInfo, TypeInfo>
// : public TypeInfo

class ClassArchetypeTypeInfo : public HeapTypeInfo<ClassArchetypeTypeInfo>;
// : public SingleScalarTypeInfoWithTypeLayout<ClassArchetypeTypeInfo, ReferenceTypeInfo>
// : public SingleScalarTypeInfo<ClassArchetypeTypeInfo, ReferenceTypeInfo>
// : public ScalarTypeInfo<ClassArchetypeTypeInfo, ReferenceTypeInfo>
// : public ReferenceTypeInfo
// : public LoadableTypeInfo
// : public FixedTypeInfo
// : public TypeInfo

class FixedSizeArchetypeTypeInfo : public PODSingleScalarTypeInfo<FixedSizeArchetypeTypeInfo, LoadableTypeInfo>;
// : public SingleScalarTypeInfo<FixedSizeArchetypeTypeInfo, LoadableTypeInfo>
// : public ScalarTypeInfo<FixedSizeArchetypeTypeInfo, LoadableTypeInfo>
// : public LoadableTypeInfo
// : public FixedTypeInfo
// : public TypeInfo

class BoxTypeInfo : public HeapTypeInfo<BoxTypeInfo>;
class EmptyBoxTypeInfo final : public BoxTypeInfo;
class NonFixedBoxTypeInfo final : public BoxTypeInfo;
class FixedBoxTypeInfoBase : public BoxTypeInfo;

class PODBoxTypeInfo final : public FixedBoxTypeInfoBase;
class SingleRefcountedBoxTypeInfo final : public FixedBoxTypeInfoBase;
class FixedBoxTypeInfo final : public FixedBoxTypeInfoBase;

class ThinFuncTypeInfo : public PODSingleScalarTypeInfo<ThinFuncTypeInfo, LoadableTypeInfo>,
// : public SingleScalarTypeInfo<ThinFuncTypeInfo, LoadableTypeInfo>
// : public ScalarTypeInfo<FixedSizeArchetypeTypeInfo, LoadableTypeInfo>
// : public LoadableTypeInfo
// : public FixedTypeInfo
// : public TypeInfo

template <class Derived, class Base>
class ScalarPairTypeInfo : public ScalarTypeInfo<Derived, Base>;
// : public Base

class FuncTypeInfo : public ScalarPairTypeInfo<FuncTypeInfo, ReferenceTypeInfo>;
// : public ScalarTypeInfo<FuncTypeInfo, ReferenceTypeInfo>
// : public ReferenceTypeInfo
// : public LoadableTypeInfo
// : public FixedTypeInfo
// : public TypeInfo

class BlockStorageTypeInfo final : public IndirectTypeInfo<BlockStorageTypeInfo, FixedTypeInfo>;
// : public FixedTypeInfo
// : public TypeInfo
