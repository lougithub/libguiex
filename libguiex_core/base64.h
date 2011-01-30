/* 
 public domain BASE64 code
 */

#ifndef __CC_BASE64_DECODE_H
#define __CC_BASE64_DECODE_H

/** @file
 base64 helper functions
 */

/** 
 * Decodes a 64base encoded memory. The decoded memory is
 * expected to be freed by the caller.
 *
 * @returns the length of the out buffer
 *
 */
int base64Decode( const unsigned char *in, unsigned int inLength, unsigned char **out);


#endif // __CC_BASE64_DECODE_H
