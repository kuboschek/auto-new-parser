#!/usr/bin/env python3

import datetime

import json
import csv
import urllib2

"""
Row names are:
    IDdec, IDhex, Name, Description, Length, Nichts,

    Field1, Field2, Field3, Field4, Field5, Field6, Field7, Field8,

    Nichts, Offset, Faktor, Min, Max, SendInterval,

    SGBack, BackSG, SGHID, HIDSG, SGDbg, DbgSG
"""

def simplify_type(type_str):
    if type_str == 'u8':
        return 'uint8_t'

    elif type_str == 'u16':
        return 'uint16_t'

    elif type_str == 'u32':
        return 'uint32_t'

    else:
        return type_str

def fetch_from_docs():
    doc_key = "1sd0Kv8KWm5AEQv1o5sLPzUiSmL4zypjoT5Xa6dOiU9E"
    doc_url = "https://docs.google.com/spreadsheet/pub?key=" + doc_key + "&output=csv"

    resp = urllib2.urlopen(doc_url)
    cr = csv.reader(resp)

    messages = []

    for row in cr:
        fields = []

        # If the name is not empty
        if row[2] != "":
            try:
                for i in range(6, 13):
                    try:
                        fstr = row[i].split(' ')
                        fields.append({'name':fstr[1], 'type':simplify_type(fstr[0])})
                    except IndexError as idx_er:
                        pass

                msg = {
                    'id':int(row[0]),
                    'name':row[2],
                    'fields':fields,
                    'generate_req':False,
                }

                messages.append(msg)
            except IndexError as idx_er:
                print(idx_er)
            except ValueError as val_er:
                print(val_er)

    return messages

def write_new_msg_def(messages):
    msg_obj = {'time':str(datetime.datetime.now()), 'messages':messages}

    with open("messages.json", 'w') as f:
        json.dump(msg_obj, f, indent=4)

msgs = fetch_from_docs()
write_new_msg_def(msgs)

print(str(len(msgs)) + " messages written to messages.json")
