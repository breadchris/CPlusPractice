#!/usr/bin/env pythonn
#

import types

# Raid's Serialized Object Notation
# or RSON
#
class BaseRSON(object):
    def __init__(self):
        pass
    def __getattr__(self, name):
        return None
    def internal_serialize(self, member):
        if isinstance(member, types.IntType):
            serialized_object = "I%x:" % member
        elif isinstance(member, types.LongType):
            serialized_object = "I%x:" % member
        elif isinstance(member, types.StringType):
            serialized_object = "S%x:%s" % (len(member), member)
        elif isinstance(member, types.BooleanType):
            if element == True:
                serialized_object = "B1"
            else:
                serialized_object = "B0"
        elif isinstance(member, types.ListType):
            member.sort()
            serialized_object = "L%x:" % len(member)
            for item in member:
                serialized_object += self.internal_serialize(item)
        elif isinstance(member, types.DictionaryType):
            serialized_object = "D%x:" % len(member)
            for key in member.keys():
                value = member[key]
                serialized_object += self.internal_serialize(key)
                serialized_object += ":"
                serialized_object += self.internal_serialize(value)
        else:
            return False
        return serialized_object

    def serialize(self):
        serialized_object = ''
        object_members = dir(self)
        for name in object_members:
            if name[0] == '_':
                continue
            member = getattr(self, name)
            if hasattr(member, "__call__"):
                continue
            if isinstance(member, types.FunctionType):
                continue
            if member in ['', False, None]:
                continue
            serialized_object += "NS%x:%s" % (len(name), name)
            serialized_object += self.internal_serialize(member)
        return serialized_object

    def internal_deserialize(self, serialized_object):
        data = serialized_object
        next_delimeter = lambda: data.index(':')
        read_advance = lambda length,data: (data[:length], data[length:])
        seek_integer = lambda length: (int(data[:length],16), data[length+1:])
        read_integer = lambda: seek_integer(next_delimeter())
        read_string = lambda: read_advance(*read_integer())
        read_type = lambda: read_advance(1, data)
        read_boolean = lambda: ({"1":True, "0":False}[data[0]],data[1:])
        data_type, data = read_type()
        if data_type == 'I':
            return read_integer()
        elif data_type == 'B':
            return read_boolean()
        elif data_type == 'S':
            return read_string()
        elif data_type == 'L':
            count, data = read_integer()
            new_list = []
            while count > 0:
                value, data = self.internal_deserialize(data)
                new_list.append(value)
                count -= 1
            new_list.sort()
            return new_list, data
        elif data_type == 'D':
            count, data = read_integer()
            new_dict = dict()
            while count > 0:
                key, data = self.internal_deserialize(data)
                sep, data = read_type()
                if sep != ":":
                    raise AttributeError 
                value, data = self.internal_deserialize(data)
                new_dict[key] = value
                count -= 1
            return new_dict, data
        else:   
            raise AttributeError

    def deserialize(self, serialized_object):
        data = serialized_object
        if True:
            while len(data):
                if data[0] != 'N':
                    return False
                name, data = self.internal_deserialize(data[1:])
                if name[0] == '_':
                    return False
                if hasattr(self, name):
                    attribute = getattr(self, name)
                    if isinstance(attribute, types.FunctionType):
                        return False
                    if hasattr(attribute, "__call__"):
                        return False
                value, data = self.internal_deserialize(data)
                setattr(self, name, value)
        else:
            return False
        return True

