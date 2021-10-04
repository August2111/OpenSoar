import sys


if len(sys.argv) > 1:
    language = sys.argv[1]

outdir = ''  # without this argument 2 write in the same directory like input file!
if len(sys.argv) > 2:
    outdir = sys.argv[2]
    outdir = outdir + '/'

if not language:
    language = 'de'

filename = language + '.mo'  # f.e. 'de.mo'
with open(filename, 'rb') as readfile:
    bytes_read = readfile.read()
    i = 0
    filelength = '0x'+ ''.join('{:08X}'.format(len(bytes_read)))  # Format 0x00000000
    LANGUAGE = language.upper()

    # C Source file :
    with open(outdir + filename +'.c', 'wt', newline='') as writefile:
        writefile.write('const unsigned char ' + language + '_mo[] = {\n')

        for b in bytes_read:
            i = i + 1
            writefile.write('0x' + ''.join('{:02X}'.format(b)) + ', ')  # Format 0x00
            if i >= 16 :
                writefile.write('\n')
                i = 0
        writefile.write('};\n\n')
        writefile.write('const unsigned long ' + language + '_mo_termination = 0x00000000;\n')
        writefile.write('const unsigned long ' + language + '_mo_start       = 0x00000000;\n')
        writefile.write('const unsigned long ' + language + '_mo_finish      = ' +  filelength + ';\n')
        writefile.write('const unsigned long ' + language + '_mo_length      = ' +  filelength + ';\n')
        writefile.write('\n')
        writefile.write('#define ' + LANGUAGE + '_MO_TERMINATION 0x00000000\n')
        writefile.write('#define ' + LANGUAGE + '_MO_START       0x00000000\n')
        writefile.write('#define ' + LANGUAGE + '_MO_FINISH      ' +  filelength + '\n')
        writefile.write('#define ' + LANGUAGE + '_MO_LENGTH      ' +  filelength + '\n')
        writefile.write('\n')
        writefile.write('const unsigned long ' + language + '_mo_size = ' + LANGUAGE + '_MO_LENGTH;\n')
        writefile.close()
    # H - Header file :
    with open(outdir + filename +'.h', 'wt', newline='') as writefile:
        writefile.write('#ifndef OUTPUT_PO_' + LANGUAGE + '_MO_H\n')
        writefile.write('#define OUTPUT_PO_' + LANGUAGE + '_MO_H\n')
        writefile.write('\n')
        writefile.write('extern const unsigned long ' + language + '_mo_termination;\n')
        writefile.write('extern const unsigned long ' + language + '_mo_start;\n')
        writefile.write('extern const unsigned long ' + language + '_mo_finish;\n')
        writefile.write('extern const unsigned long ' + language + '_mo_length;\n')
        writefile.write('extern const unsigned char ' + language + '_mo[];\n')
        writefile.write('\n')
        writefile.write('#endif  // OUTPUT_PO_' + LANGUAGE + '_MO_H\n')

        writefile.close()
readfile.close()


