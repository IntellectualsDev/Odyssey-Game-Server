import * as flatbuffers from 'flatbuffers';
import { MonsterT as MyGame_Example2_MonsterT } from '../example2/monster.js';
import { Ability, AbilityT } from './ability.js';
import { Any } from './any.js';
import { AnyAmbiguousAliases } from './any-ambiguous-aliases.js';
import { AnyUniqueAliases } from './any-unique-aliases.js';
import { Color } from './color.js';
import { Race } from './race.js';
import { Referrable, ReferrableT } from './referrable.js';
import { Stat, StatT } from './stat.js';
import { Test, TestT } from './test.js';
import { TestSimpleTableWithEnumT } from './test-simple-table-with-enum.js';
import { Vec3, Vec3T } from './vec3.js';
import { InParentNamespace, InParentNamespaceT } from '../in-parent-namespace.js';
/**
 * an example documentation comment: "monster object"
 */
export declare class Monster implements flatbuffers.IUnpackableObject<MonsterT> {
    bb: flatbuffers.ByteBuffer | null;
    bb_pos: number;
    __init(i: number, bb: flatbuffers.ByteBuffer): Monster;
    static getRootAsMonster(bb: flatbuffers.ByteBuffer, obj?: Monster): Monster;
    static getSizePrefixedRootAsMonster(bb: flatbuffers.ByteBuffer, obj?: Monster): Monster;
    static bufferHasIdentifier(bb: flatbuffers.ByteBuffer): boolean;
    pos(obj?: Vec3): Vec3 | null;
    mana(): number;
    mutate_mana(value: number): boolean;
    hp(): number;
    mutate_hp(value: number): boolean;
    name(): string | null;
    name(optionalEncoding: flatbuffers.Encoding): string | Uint8Array | null;
    inventory(index: number): number | null;
    inventoryLength(): number;
    inventoryArray(): Uint8Array | null;
    color(): Color;
    mutate_color(value: Color): boolean;
    testType(): Any;
    test<T extends flatbuffers.Table>(obj: any): any | null;
    test4(index: number, obj?: Test): Test | null;
    test4Length(): number;
    testarrayofstring(index: number): string;
    testarrayofstring(index: number, optionalEncoding: flatbuffers.Encoding): string | Uint8Array;
    testarrayofstringLength(): number;
    /**
     * an example documentation comment: this will end up in the generated code
     * multiline too
     */
    testarrayoftables(index: number, obj?: Monster): Monster | null;
    testarrayoftablesLength(): number;
    enemy(obj?: Monster): Monster | null;
    testnestedflatbuffer(index: number): number | null;
    testnestedflatbufferLength(): number;
    testnestedflatbufferArray(): Uint8Array | null;
    testempty(obj?: Stat): Stat | null;
    testbool(): boolean;
    mutate_testbool(value: boolean): boolean;
    testhashs32Fnv1(): number;
    mutate_testhashs32_fnv1(value: number): boolean;
    testhashu32Fnv1(): number;
    mutate_testhashu32_fnv1(value: number): boolean;
    testhashs64Fnv1(): bigint;
    mutate_testhashs64_fnv1(value: bigint): boolean;
    testhashu64Fnv1(): bigint;
    mutate_testhashu64_fnv1(value: bigint): boolean;
    testhashs32Fnv1a(): number;
    mutate_testhashs32_fnv1a(value: number): boolean;
    testhashu32Fnv1a(): number;
    mutate_testhashu32_fnv1a(value: number): boolean;
    testhashs64Fnv1a(): bigint;
    mutate_testhashs64_fnv1a(value: bigint): boolean;
    testhashu64Fnv1a(): bigint;
    mutate_testhashu64_fnv1a(value: bigint): boolean;
    testarrayofbools(index: number): boolean | null;
    testarrayofboolsLength(): number;
    testarrayofboolsArray(): Int8Array | null;
    testf(): number;
    mutate_testf(value: number): boolean;
    testf2(): number;
    mutate_testf2(value: number): boolean;
    testf3(): number;
    mutate_testf3(value: number): boolean;
    testarrayofstring2(index: number): string;
    testarrayofstring2(index: number, optionalEncoding: flatbuffers.Encoding): string | Uint8Array;
    testarrayofstring2Length(): number;
    testarrayofsortedstruct(index: number, obj?: Ability): Ability | null;
    testarrayofsortedstructLength(): number;
    flex(index: number): number | null;
    flexLength(): number;
    flexArray(): Uint8Array | null;
    test5(index: number, obj?: Test): Test | null;
    test5Length(): number;
    vectorOfLongs(index: number): bigint | null;
    vectorOfLongsLength(): number;
    vectorOfDoubles(index: number): number | null;
    vectorOfDoublesLength(): number;
    vectorOfDoublesArray(): Float64Array | null;
    parentNamespaceTest(obj?: InParentNamespace): InParentNamespace | null;
    vectorOfReferrables(index: number, obj?: Referrable): Referrable | null;
    vectorOfReferrablesLength(): number;
    singleWeakReference(): bigint;
    mutate_single_weak_reference(value: bigint): boolean;
    vectorOfWeakReferences(index: number): bigint | null;
    vectorOfWeakReferencesLength(): number;
    vectorOfStrongReferrables(index: number, obj?: Referrable): Referrable | null;
    vectorOfStrongReferrablesLength(): number;
    coOwningReference(): bigint;
    mutate_co_owning_reference(value: bigint): boolean;
    vectorOfCoOwningReferences(index: number): bigint | null;
    vectorOfCoOwningReferencesLength(): number;
    nonOwningReference(): bigint;
    mutate_non_owning_reference(value: bigint): boolean;
    vectorOfNonOwningReferences(index: number): bigint | null;
    vectorOfNonOwningReferencesLength(): number;
    anyUniqueType(): AnyUniqueAliases;
    anyUnique<T extends flatbuffers.Table>(obj: any): any | null;
    anyAmbiguousType(): AnyAmbiguousAliases;
    anyAmbiguous<T extends flatbuffers.Table>(obj: any): any | null;
    vectorOfEnums(index: number): Color | null;
    vectorOfEnumsLength(): number;
    vectorOfEnumsArray(): Uint8Array | null;
    signedEnum(): Race;
    mutate_signed_enum(value: Race): boolean;
    testrequirednestedflatbuffer(index: number): number | null;
    testrequirednestedflatbufferLength(): number;
    testrequirednestedflatbufferArray(): Uint8Array | null;
    scalarKeySortedTables(index: number, obj?: Stat): Stat | null;
    scalarKeySortedTablesLength(): number;
    nativeInline(obj?: Test): Test | null;
    longEnumNonEnumDefault(): bigint;
    mutate_long_enum_non_enum_default(value: bigint): boolean;
    longEnumNormalDefault(): bigint;
    mutate_long_enum_normal_default(value: bigint): boolean;
    nanDefault(): number;
    mutate_nan_default(value: number): boolean;
    infDefault(): number;
    mutate_inf_default(value: number): boolean;
    positiveInfDefault(): number;
    mutate_positive_inf_default(value: number): boolean;
    infinityDefault(): number;
    mutate_infinity_default(value: number): boolean;
    positiveInfinityDefault(): number;
    mutate_positive_infinity_default(value: number): boolean;
    negativeInfDefault(): number;
    mutate_negative_inf_default(value: number): boolean;
    negativeInfinityDefault(): number;
    mutate_negative_infinity_default(value: number): boolean;
    doubleInfDefault(): number;
    mutate_double_inf_default(value: number): boolean;
    static getFullyQualifiedName(): string;
    static startMonster(builder: flatbuffers.Builder): void;
    static addPos(builder: flatbuffers.Builder, posOffset: flatbuffers.Offset): void;
    static addMana(builder: flatbuffers.Builder, mana: number): void;
    static addHp(builder: flatbuffers.Builder, hp: number): void;
    static addName(builder: flatbuffers.Builder, nameOffset: flatbuffers.Offset): void;
    static addInventory(builder: flatbuffers.Builder, inventoryOffset: flatbuffers.Offset): void;
    static createInventoryVector(builder: flatbuffers.Builder, data: number[] | Uint8Array): flatbuffers.Offset;
    static startInventoryVector(builder: flatbuffers.Builder, numElems: number): void;
    static addColor(builder: flatbuffers.Builder, color: Color): void;
    static addTestType(builder: flatbuffers.Builder, testType: Any): void;
    static addTest(builder: flatbuffers.Builder, testOffset: flatbuffers.Offset): void;
    static addTest4(builder: flatbuffers.Builder, test4Offset: flatbuffers.Offset): void;
    static startTest4Vector(builder: flatbuffers.Builder, numElems: number): void;
    static addTestarrayofstring(builder: flatbuffers.Builder, testarrayofstringOffset: flatbuffers.Offset): void;
    static createTestarrayofstringVector(builder: flatbuffers.Builder, data: flatbuffers.Offset[]): flatbuffers.Offset;
    static startTestarrayofstringVector(builder: flatbuffers.Builder, numElems: number): void;
    static addTestarrayoftables(builder: flatbuffers.Builder, testarrayoftablesOffset: flatbuffers.Offset): void;
    static createTestarrayoftablesVector(builder: flatbuffers.Builder, data: flatbuffers.Offset[]): flatbuffers.Offset;
    static startTestarrayoftablesVector(builder: flatbuffers.Builder, numElems: number): void;
    static addEnemy(builder: flatbuffers.Builder, enemyOffset: flatbuffers.Offset): void;
    static addTestnestedflatbuffer(builder: flatbuffers.Builder, testnestedflatbufferOffset: flatbuffers.Offset): void;
    static createTestnestedflatbufferVector(builder: flatbuffers.Builder, data: number[] | Uint8Array): flatbuffers.Offset;
    static startTestnestedflatbufferVector(builder: flatbuffers.Builder, numElems: number): void;
    static addTestempty(builder: flatbuffers.Builder, testemptyOffset: flatbuffers.Offset): void;
    static addTestbool(builder: flatbuffers.Builder, testbool: boolean): void;
    static addTesthashs32Fnv1(builder: flatbuffers.Builder, testhashs32Fnv1: number): void;
    static addTesthashu32Fnv1(builder: flatbuffers.Builder, testhashu32Fnv1: number): void;
    static addTesthashs64Fnv1(builder: flatbuffers.Builder, testhashs64Fnv1: bigint): void;
    static addTesthashu64Fnv1(builder: flatbuffers.Builder, testhashu64Fnv1: bigint): void;
    static addTesthashs32Fnv1a(builder: flatbuffers.Builder, testhashs32Fnv1a: number): void;
    static addTesthashu32Fnv1a(builder: flatbuffers.Builder, testhashu32Fnv1a: number): void;
    static addTesthashs64Fnv1a(builder: flatbuffers.Builder, testhashs64Fnv1a: bigint): void;
    static addTesthashu64Fnv1a(builder: flatbuffers.Builder, testhashu64Fnv1a: bigint): void;
    static addTestarrayofbools(builder: flatbuffers.Builder, testarrayofboolsOffset: flatbuffers.Offset): void;
    static createTestarrayofboolsVector(builder: flatbuffers.Builder, data: boolean[]): flatbuffers.Offset;
    static startTestarrayofboolsVector(builder: flatbuffers.Builder, numElems: number): void;
    static addTestf(builder: flatbuffers.Builder, testf: number): void;
    static addTestf2(builder: flatbuffers.Builder, testf2: number): void;
    static addTestf3(builder: flatbuffers.Builder, testf3: number): void;
    static addTestarrayofstring2(builder: flatbuffers.Builder, testarrayofstring2Offset: flatbuffers.Offset): void;
    static createTestarrayofstring2Vector(builder: flatbuffers.Builder, data: flatbuffers.Offset[]): flatbuffers.Offset;
    static startTestarrayofstring2Vector(builder: flatbuffers.Builder, numElems: number): void;
    static addTestarrayofsortedstruct(builder: flatbuffers.Builder, testarrayofsortedstructOffset: flatbuffers.Offset): void;
    static startTestarrayofsortedstructVector(builder: flatbuffers.Builder, numElems: number): void;
    static addFlex(builder: flatbuffers.Builder, flexOffset: flatbuffers.Offset): void;
    static createFlexVector(builder: flatbuffers.Builder, data: number[] | Uint8Array): flatbuffers.Offset;
    static startFlexVector(builder: flatbuffers.Builder, numElems: number): void;
    static addTest5(builder: flatbuffers.Builder, test5Offset: flatbuffers.Offset): void;
    static startTest5Vector(builder: flatbuffers.Builder, numElems: number): void;
    static addVectorOfLongs(builder: flatbuffers.Builder, vectorOfLongsOffset: flatbuffers.Offset): void;
    static createVectorOfLongsVector(builder: flatbuffers.Builder, data: bigint[]): flatbuffers.Offset;
    static startVectorOfLongsVector(builder: flatbuffers.Builder, numElems: number): void;
    static addVectorOfDoubles(builder: flatbuffers.Builder, vectorOfDoublesOffset: flatbuffers.Offset): void;
    static createVectorOfDoublesVector(builder: flatbuffers.Builder, data: number[] | Float64Array): flatbuffers.Offset;
    /**
     * @deprecated This Uint8Array overload will be removed in the future.
     */
    static createVectorOfDoublesVector(builder: flatbuffers.Builder, data: number[] | Uint8Array): flatbuffers.Offset;
    static startVectorOfDoublesVector(builder: flatbuffers.Builder, numElems: number): void;
    static addParentNamespaceTest(builder: flatbuffers.Builder, parentNamespaceTestOffset: flatbuffers.Offset): void;
    static addVectorOfReferrables(builder: flatbuffers.Builder, vectorOfReferrablesOffset: flatbuffers.Offset): void;
    static createVectorOfReferrablesVector(builder: flatbuffers.Builder, data: flatbuffers.Offset[]): flatbuffers.Offset;
    static startVectorOfReferrablesVector(builder: flatbuffers.Builder, numElems: number): void;
    static addSingleWeakReference(builder: flatbuffers.Builder, singleWeakReference: bigint): void;
    static addVectorOfWeakReferences(builder: flatbuffers.Builder, vectorOfWeakReferencesOffset: flatbuffers.Offset): void;
    static createVectorOfWeakReferencesVector(builder: flatbuffers.Builder, data: bigint[]): flatbuffers.Offset;
    static startVectorOfWeakReferencesVector(builder: flatbuffers.Builder, numElems: number): void;
    static addVectorOfStrongReferrables(builder: flatbuffers.Builder, vectorOfStrongReferrablesOffset: flatbuffers.Offset): void;
    static createVectorOfStrongReferrablesVector(builder: flatbuffers.Builder, data: flatbuffers.Offset[]): flatbuffers.Offset;
    static startVectorOfStrongReferrablesVector(builder: flatbuffers.Builder, numElems: number): void;
    static addCoOwningReference(builder: flatbuffers.Builder, coOwningReference: bigint): void;
    static addVectorOfCoOwningReferences(builder: flatbuffers.Builder, vectorOfCoOwningReferencesOffset: flatbuffers.Offset): void;
    static createVectorOfCoOwningReferencesVector(builder: flatbuffers.Builder, data: bigint[]): flatbuffers.Offset;
    static startVectorOfCoOwningReferencesVector(builder: flatbuffers.Builder, numElems: number): void;
    static addNonOwningReference(builder: flatbuffers.Builder, nonOwningReference: bigint): void;
    static addVectorOfNonOwningReferences(builder: flatbuffers.Builder, vectorOfNonOwningReferencesOffset: flatbuffers.Offset): void;
    static createVectorOfNonOwningReferencesVector(builder: flatbuffers.Builder, data: bigint[]): flatbuffers.Offset;
    static startVectorOfNonOwningReferencesVector(builder: flatbuffers.Builder, numElems: number): void;
    static addAnyUniqueType(builder: flatbuffers.Builder, anyUniqueType: AnyUniqueAliases): void;
    static addAnyUnique(builder: flatbuffers.Builder, anyUniqueOffset: flatbuffers.Offset): void;
    static addAnyAmbiguousType(builder: flatbuffers.Builder, anyAmbiguousType: AnyAmbiguousAliases): void;
    static addAnyAmbiguous(builder: flatbuffers.Builder, anyAmbiguousOffset: flatbuffers.Offset): void;
    static addVectorOfEnums(builder: flatbuffers.Builder, vectorOfEnumsOffset: flatbuffers.Offset): void;
    static createVectorOfEnumsVector(builder: flatbuffers.Builder, data: Color[]): flatbuffers.Offset;
    static startVectorOfEnumsVector(builder: flatbuffers.Builder, numElems: number): void;
    static addSignedEnum(builder: flatbuffers.Builder, signedEnum: Race): void;
    static addTestrequirednestedflatbuffer(builder: flatbuffers.Builder, testrequirednestedflatbufferOffset: flatbuffers.Offset): void;
    static createTestrequirednestedflatbufferVector(builder: flatbuffers.Builder, data: number[] | Uint8Array): flatbuffers.Offset;
    static startTestrequirednestedflatbufferVector(builder: flatbuffers.Builder, numElems: number): void;
    static addScalarKeySortedTables(builder: flatbuffers.Builder, scalarKeySortedTablesOffset: flatbuffers.Offset): void;
    static createScalarKeySortedTablesVector(builder: flatbuffers.Builder, data: flatbuffers.Offset[]): flatbuffers.Offset;
    static startScalarKeySortedTablesVector(builder: flatbuffers.Builder, numElems: number): void;
    static addNativeInline(builder: flatbuffers.Builder, nativeInlineOffset: flatbuffers.Offset): void;
    static addLongEnumNonEnumDefault(builder: flatbuffers.Builder, longEnumNonEnumDefault: bigint): void;
    static addLongEnumNormalDefault(builder: flatbuffers.Builder, longEnumNormalDefault: bigint): void;
    static addNanDefault(builder: flatbuffers.Builder, nanDefault: number): void;
    static addInfDefault(builder: flatbuffers.Builder, infDefault: number): void;
    static addPositiveInfDefault(builder: flatbuffers.Builder, positiveInfDefault: number): void;
    static addInfinityDefault(builder: flatbuffers.Builder, infinityDefault: number): void;
    static addPositiveInfinityDefault(builder: flatbuffers.Builder, positiveInfinityDefault: number): void;
    static addNegativeInfDefault(builder: flatbuffers.Builder, negativeInfDefault: number): void;
    static addNegativeInfinityDefault(builder: flatbuffers.Builder, negativeInfinityDefault: number): void;
    static addDoubleInfDefault(builder: flatbuffers.Builder, doubleInfDefault: number): void;
    static endMonster(builder: flatbuffers.Builder): flatbuffers.Offset;
    static finishMonsterBuffer(builder: flatbuffers.Builder, offset: flatbuffers.Offset): void;
    static finishSizePrefixedMonsterBuffer(builder: flatbuffers.Builder, offset: flatbuffers.Offset): void;
    serialize(): Uint8Array;
    static deserialize(buffer: Uint8Array): Monster;
    unpack(): MonsterT;
    unpackTo(_o: MonsterT): void;
}
export declare class MonsterT implements flatbuffers.IGeneratedObject {
    pos: Vec3T | null;
    mana: number;
    hp: number;
    name: string | Uint8Array | null;
    inventory: (number)[];
    color: Color;
    testType: Any;
    test: MonsterT | MyGame_Example2_MonsterT | TestSimpleTableWithEnumT | null;
    test4: (TestT)[];
    testarrayofstring: (string)[];
    testarrayoftables: (MonsterT)[];
    enemy: MonsterT | null;
    testnestedflatbuffer: (number)[];
    testempty: StatT | null;
    testbool: boolean;
    testhashs32Fnv1: number;
    testhashu32Fnv1: number;
    testhashs64Fnv1: bigint;
    testhashu64Fnv1: bigint;
    testhashs32Fnv1a: number;
    testhashu32Fnv1a: number;
    testhashs64Fnv1a: bigint;
    testhashu64Fnv1a: bigint;
    testarrayofbools: (boolean)[];
    testf: number;
    testf2: number;
    testf3: number;
    testarrayofstring2: (string)[];
    testarrayofsortedstruct: (AbilityT)[];
    flex: (number)[];
    test5: (TestT)[];
    vectorOfLongs: (bigint)[];
    vectorOfDoubles: (number)[];
    parentNamespaceTest: InParentNamespaceT | null;
    vectorOfReferrables: (ReferrableT)[];
    singleWeakReference: bigint;
    vectorOfWeakReferences: (bigint)[];
    vectorOfStrongReferrables: (ReferrableT)[];
    coOwningReference: bigint;
    vectorOfCoOwningReferences: (bigint)[];
    nonOwningReference: bigint;
    vectorOfNonOwningReferences: (bigint)[];
    anyUniqueType: AnyUniqueAliases;
    anyUnique: MonsterT | MyGame_Example2_MonsterT | TestSimpleTableWithEnumT | null;
    anyAmbiguousType: AnyAmbiguousAliases;
    anyAmbiguous: MonsterT | null;
    vectorOfEnums: (Color)[];
    signedEnum: Race;
    testrequirednestedflatbuffer: (number)[];
    scalarKeySortedTables: (StatT)[];
    nativeInline: TestT | null;
    longEnumNonEnumDefault: bigint;
    longEnumNormalDefault: bigint;
    nanDefault: number;
    infDefault: number;
    positiveInfDefault: number;
    infinityDefault: number;
    positiveInfinityDefault: number;
    negativeInfDefault: number;
    negativeInfinityDefault: number;
    doubleInfDefault: number;
    constructor(pos?: Vec3T | null, mana?: number, hp?: number, name?: string | Uint8Array | null, inventory?: (number)[], color?: Color, testType?: Any, test?: MonsterT | MyGame_Example2_MonsterT | TestSimpleTableWithEnumT | null, test4?: (TestT)[], testarrayofstring?: (string)[], testarrayoftables?: (MonsterT)[], enemy?: MonsterT | null, testnestedflatbuffer?: (number)[], testempty?: StatT | null, testbool?: boolean, testhashs32Fnv1?: number, testhashu32Fnv1?: number, testhashs64Fnv1?: bigint, testhashu64Fnv1?: bigint, testhashs32Fnv1a?: number, testhashu32Fnv1a?: number, testhashs64Fnv1a?: bigint, testhashu64Fnv1a?: bigint, testarrayofbools?: (boolean)[], testf?: number, testf2?: number, testf3?: number, testarrayofstring2?: (string)[], testarrayofsortedstruct?: (AbilityT)[], flex?: (number)[], test5?: (TestT)[], vectorOfLongs?: (bigint)[], vectorOfDoubles?: (number)[], parentNamespaceTest?: InParentNamespaceT | null, vectorOfReferrables?: (ReferrableT)[], singleWeakReference?: bigint, vectorOfWeakReferences?: (bigint)[], vectorOfStrongReferrables?: (ReferrableT)[], coOwningReference?: bigint, vectorOfCoOwningReferences?: (bigint)[], nonOwningReference?: bigint, vectorOfNonOwningReferences?: (bigint)[], anyUniqueType?: AnyUniqueAliases, anyUnique?: MonsterT | MyGame_Example2_MonsterT | TestSimpleTableWithEnumT | null, anyAmbiguousType?: AnyAmbiguousAliases, anyAmbiguous?: MonsterT | null, vectorOfEnums?: (Color)[], signedEnum?: Race, testrequirednestedflatbuffer?: (number)[], scalarKeySortedTables?: (StatT)[], nativeInline?: TestT | null, longEnumNonEnumDefault?: bigint, longEnumNormalDefault?: bigint, nanDefault?: number, infDefault?: number, positiveInfDefault?: number, infinityDefault?: number, positiveInfinityDefault?: number, negativeInfDefault?: number, negativeInfinityDefault?: number, doubleInfDefault?: number);
    pack(builder: flatbuffers.Builder): flatbuffers.Offset;
}
