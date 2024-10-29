#ifndef JSON_H
#define JSON_H

#include "vec.h"

Vec* prettify_json(char* json) {
    Vec* pretty_json = vec_new(0, sizeof(char));

    if (pretty_json == NULL) {
        fprintf(stderr, "vec_new() failed\n");
        return NULL;
    }

    int indent = 0;
    int in_string = 0;

    for (char* current_char = json; *current_char != 0; current_char++) {
        switch (*current_char) {
            case '\"':
                in_string = !in_string; 
                vec_push(&pretty_json, current_char, sizeof(char));
                break;
            case '{':
            case '[':
                if (!in_string) {
                    vec_push(&pretty_json, current_char, sizeof(char));
                    vec_push(&pretty_json, "\n", sizeof(char));
                    indent++;
                    for (int i = 0; i < indent; i++) {
                        vec_push(&pretty_json, "\t", sizeof(char));
                    }
                } else {
                    vec_push(&pretty_json, current_char, sizeof(char));
                }
                break;
            case '}':
            case ']':
                if (!in_string) {
                    vec_push(&pretty_json, "\n", sizeof(char));
                    indent--;
                    for (int i = 0; i < indent; i++) {
                        vec_push(&pretty_json, "\t", sizeof(char));
                    }
                    vec_push(&pretty_json, current_char, sizeof(char));
                } else {
                    vec_push(&pretty_json, current_char, sizeof(char));
                }
                break;
            case ',':
                vec_push(&pretty_json, current_char, sizeof(char));
                if (!in_string) {
                    vec_push(&pretty_json, "\n", sizeof(char));
                    for (int i = 0; i < indent; i++) {
                        vec_push(&pretty_json, "\t", sizeof(char));
                    }
                }
                break;
            case ':':
                vec_push(&pretty_json, current_char, sizeof(char));
                if (!in_string) {
                    vec_push(&pretty_json, " ", sizeof(char));
                }
                break;
            default:
                vec_push(&pretty_json, current_char, sizeof(char));
                break;
        }
    }

    vec_push(&pretty_json, "\n", sizeof(char));
    vec_push(&pretty_json, "\0", sizeof(char));

    return pretty_json;
}


#endif