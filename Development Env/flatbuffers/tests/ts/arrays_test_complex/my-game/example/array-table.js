// automatically generated by the FlatBuffers compiler, do not modify
/* eslint-disable @typescript-eslint/no-unused-vars, @typescript-eslint/no-explicit-any, @typescript-eslint/no-non-null-assertion */
import * as flatbuffers from 'flatbuffers';
import { ArrayStruct } from './array-struct.d.ts';
export class ArrayTable {
    constructor() {
        this.bb = null;
        this.bb_pos = 0;
    }
    __init(i, bb) {
        this.bb_pos = i;
        this.bb = bb;
        return this;
    }
    static getRootAsArrayTable(bb, obj) {
        return (obj || new ArrayTable()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
    }
    static getSizePrefixedRootAsArrayTable(bb, obj) {
        bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
        return (obj || new ArrayTable()).__init(bb.readInt32(bb.position()) + bb.position(), bb);
    }
    static bufferHasIdentifier(bb) {
        return bb.__has_identifier('RHUB');
    }
    a(optionalEncoding) {
        const offset = this.bb.__offset(this.bb_pos, 4);
        return offset ? this.bb.__string(this.bb_pos + offset, optionalEncoding) : null;
    }
    cUnderscore(obj) {
        const offset = this.bb.__offset(this.bb_pos, 6);
        return offset ? (obj || new ArrayStruct()).__init(this.bb_pos + offset, this.bb) : null;
    }
    static getFullyQualifiedName() {
        return 'MyGame.Example.ArrayTable';
    }
    static startArrayTable(builder) {
        builder.startObject(2);
    }
    static addA(builder, aOffset) {
        builder.addFieldOffset(0, aOffset, 0);
    }
    static addCUnderscore(builder, cUnderscoreOffset) {
        builder.addFieldStruct(1, cUnderscoreOffset, 0);
    }
    static endArrayTable(builder) {
        const offset = builder.endObject();
        return offset;
    }
    static finishArrayTableBuffer(builder, offset) {
        builder.finish(offset, 'RHUB');
    }
    static finishSizePrefixedArrayTableBuffer(builder, offset) {
        builder.finish(offset, 'RHUB', true);
    }
    unpack() {
        return new ArrayTableT(this.a(), (this.cUnderscore() !== null ? this.cUnderscore().unpack() : null));
    }
    unpackTo(_o) {
        _o.a = this.a();
        _o.cUnderscore = (this.cUnderscore() !== null ? this.cUnderscore().unpack() : null);
    }
}
export class ArrayTableT {
    constructor(a = null, cUnderscore = null) {
        this.a = a;
        this.cUnderscore = cUnderscore;
    }
    pack(builder) {
        const a = (this.a !== null ? builder.createString(this.a) : 0);
        ArrayTable.startArrayTable(builder);
        ArrayTable.addA(builder, a);
        ArrayTable.addCUnderscore(builder, (this.cUnderscore !== null ? this.cUnderscore.pack(builder) : 0));
        return ArrayTable.endArrayTable(builder);
    }
}
