// idSaveDecompressor_v0.1a by SamPT
#include "idSaveDecompressor.h"

int decompressSaveData(char* filename, char* destFilename, const int idchunksize) {

    FILE* f;
    byte* input;
    byte* output;
    byte* header;
    char buff[4];
    uint32 cc;
    uint32 packed_size;
    const uint32 hdrsize = 4;
    const uint32 magic = 20;

    auto oodle = LoadLibraryA("./oo2core_8_win64.dll");
    OodLZ_Decompress = (OodLZ_DecompressFunc*)GetProcAddress(oodle, "OodleLZ_Decompress");

    if (!OodLZ_Decompress) {
        printf("Error: failed to load oo2core_8_win64.dll.\n\n");
        return 1;
    }
    if (fopen_s(&f, filename, "rb") != 0) {
        printf("Error: failed to open source file for reading.\n\n");
        return 1;
    }

    fseek(f, 0, SEEK_SET);
    if (fread(buff, 1, hdrsize, f) != hdrsize) {
        printf("Error: unable to read file header.\n\n");
        return 1;
    }

    cc = *(uint32*)buff;
    if (cc == 6) {
        // Most likely a decompressed PS4 save. Re-route to compress function.
        fclose(f);
        return 2;
    }

    fseek(f, 0, SEEK_SET);
    header = new byte[magic];
    fread(header, 1, magic, f);

    fseek(f, 0, SEEK_END);
    packed_size = ftell(f);

    if (packed_size > 0x240000) {
        printf("Error: file is too large.\n\n");
        return 1;
    }

    // game_duration.dat stores data in multiple compressed "chunks"
    // Information about each chunk is stored in a table at the end of the file.
    // The offset of this data table is stored in bytes 20-23.
    fseek(f, magic, SEEK_SET);
    fread(buff, 4, 1, f);
    uint32 table_offset = ntohl(*(uint32*)buff);

    if (table_offset >= packed_size) {
        printf("Error: invalid table offset.\n\n");
        return 1;
    }

    // First 4 bytes of table = total # of data chunks in the file.
    fseek(f, table_offset, SEEK_SET);
    fread(buff, 4, 1, f);
    uint32 chunk_count = ntohl(*(uint32*)buff);

    // Define arrays to populate while looping through data
    std::vector<byte*> a_input(chunk_count);
    std::vector<byte*> a_output(chunk_count);
    std::vector<uint32> a_outbytes(chunk_count);
    std::vector<uint32> a_packed_size(chunk_count);
    std::vector<uint32> a_unpacked_size(chunk_count);
    std::vector<uint32> a_offset(chunk_count);
    a_offset[0] = hdrsize + magic;

    // Loop through data tables, generating list of compressed & uncompressed sizes.
    // Compressed sizes are used to calculate offsets for the next data chunk.
    // Uncompressed size is always 131,072 bytes, except the last one is smaller.
    for (cc = 0; cc < chunk_count; cc++) {

        if (cc > 0) {
            a_offset[cc] = a_offset[cc-1] + a_packed_size[cc-1];
        }

        fread(buff, 4, 1, f);
        a_unpacked_size[cc] = ntohl(*(uint32*)buff);
        fread(buff, 4, 1, f);
        a_packed_size[cc] = ntohl(*(uint32*)buff);
        fseek(f, 4, SEEK_CUR);

        if (a_unpacked_size[cc] > idchunksize) {
            printf("Error: invalid table contents.\n\n");
            return 1;
        }
    }

    // Read compressed data into memory
    for (cc = 0; cc < chunk_count; cc++) {
        fseek(f, a_offset[cc], SEEK_SET);
        input = new byte[a_packed_size[cc]];
        if (fread(input, 1, a_packed_size[cc], f) != (a_packed_size[cc])) {
            printf("Error: unable to read compressed data.\n\n");
            return 1;
        }
        a_input[cc] = input;
    }
    fclose(f);

    // Decompress using Oodle DLL
    for (cc = 0; cc < chunk_count; cc++) {
        output = new byte[a_unpacked_size[cc] + SAFE_SPACE];
        if (!output) {
            printf("Error: failed to allocate memory for output.\n\n");
            return 1;
        }
        a_outbytes[cc] = OodLZ_Decompress(a_input[cc], a_unpacked_size[cc], output, a_unpacked_size[cc], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        if (!a_outbytes[cc]) {
            printf("Error: failed to decompress with Oodle DLL.\n\n");
            return 1;
        }
        a_output[cc] = output;
    }

    if (fopen_s(&f, destFilename, "wb") != 0) {
        printf("Error: failed to open destination file for writing.\n\n");
        return 1;
    }

    // Write to file
    if (f != NULL) {
        char signature[25] = "IDSAVEDECOMPRESSOR_v0.1a"; // Identify compressed file for idSaveDecompressor_Auto.bat
        fwrite(signature, 24, 1, f); 
        fwrite(header, 1, magic, f);
        for (cc = 0; cc < chunk_count; cc++) {
            fwrite(a_output[cc], 1, a_outbytes[cc], f);
        };
        fclose(f);
    }

    printf("File unpacked successfully.\n\n");
    return 0;
}

int compressSaveData(char* filename, char* destFilename, const int idchunksize) {

    FILE* f;
    byte* header;
    byte* signature;
    char buff[4];
    uint32 cc;
    uint32 chunk_count;
    uint32 size;
    uint32 compressed_size;
    uint32 bytes_read;
    uint32 bytes_remaining;
    int siglen = 24;
    const int hdrsize = 4;
    const int magic = 20;
    bool flag_console = 0;

    auto oodle = LoadLibraryA("./oo2core_8_win64.dll");
    OodLZ_Compress = (OodLZ_CompressFunc*)GetProcAddress(oodle, "OodleLZ_Compress");

    if (!OodLZ_Compress) {
        printf("Failed to open oo2core_8_win64.dll\n\n");
        return 1;
    }

    if (fopen_s(&f, filename, "rb") != 0) {
        printf("Failed to open %s for reading!\n\n", filename);
        return 1;
    }

    // Check for file signature
    fseek(f, 0, SEEK_SET);
    signature = new byte[siglen + 1];
    if (fread(signature, 1, (siglen + 1), f) != (siglen + 1)) {
        printf("Error: unable to read file header.\n\n");
        return 1;
    }
    
    std::string readsig(reinterpret_cast<char const*>(signature));
    size_t strcheck = readsig.find("IDSAVEDECOMPRESSOR");
    
    // Missing file signature. Check if this is a PS4 save.
    if (strcheck == -1) {
        siglen = 0;
        fseek(f, 0, SEEK_SET);
        fread(buff, 1, hdrsize, f);
        cc = *(uint32*)buff;
        if (cc == 6) flag_console = 1;
    }

    fseek(f, 0, SEEK_END);
    size = ftell(f) - (magic + siglen);

    fseek(f, siglen, SEEK_SET);
    header = new byte[magic];
    fread(header, 1, magic, f);

    // If this is a PS4 save, we need to endian-swap the header to match PC format.
    if (flag_console) {      
        std::swap(header[0], header[3]);
        std::swap(header[16], header[19]);
        std::swap(header[17], header[18]); 
    }

    // Count is total file size divided by 131072, rounded up.
    chunk_count = (int)ceil((double)(size) / (double)(idchunksize));

    // Define arrays to populate while looping through data
    std::vector<byte*> a_input(chunk_count);
    std::vector<byte*> a_output(chunk_count);
    std::vector<uint32> a_uncompressed_size(chunk_count);
    std::vector<uint32> a_compressed_size(chunk_count);
    std::vector<uint32> a_hash(chunk_count);

    // Loop through and gather uncompressed data
    for (cc = 0; cc < chunk_count; cc++) {

        bytes_read = idchunksize * cc;
        bytes_remaining = size - bytes_read;

        if (bytes_remaining >= idchunksize)
            a_uncompressed_size[cc] = idchunksize;
        else
            a_uncompressed_size[cc] = bytes_remaining;
     
        a_input[cc] = new byte[a_uncompressed_size[cc]];
        if (fread(a_input[cc], 1, a_uncompressed_size[cc], f) != a_uncompressed_size[cc]) {
            printf("Error: unable to read uncompressed data.\n\n");
            return 1;
        }
    }

    fclose(f);

    // Compress using Oodle DLL
    for (cc = 0; cc < chunk_count; cc++) {

        // memory allocation for writing    
         a_output[cc] = new byte[size + 65536];
        *(uint32*)a_output[cc] = size;

        if (!a_output[cc]) {
            printf("Error: Couldn't allocate memory for compression.\n");
            return 1;
        }

        // 9 = mermaid, 4 = compression level
        a_compressed_size[cc] = OodLZ_Compress(9, a_input[cc], a_uncompressed_size[cc], a_output[cc], 4, 0, 0, 0, 0, 0);

        if (a_compressed_size[cc] < 0) {
            printf("Error: Compression failed.\n");
            return 1;
        }
    }

    if (fopen_s(&f, destFilename, "wb") != 0) {
        printf("Error: failed to open destination file for writing!\n");
        return 1;
    }

    // Write compressed data to file
    if (f != NULL) {

        fwrite(header, 1, magic, f); // file header
        fwrite(&hdrsize, sizeof(uint32), 1, f); // placeholder

        // compressed data
        for (cc = 0; cc < chunk_count; cc++) {
            fwrite(a_output[cc], 1, a_compressed_size[cc], f);
            char * hash = (char*)a_output[cc];
            a_hash[cc] = murmur_hash(hash, a_compressed_size[cc]);
        }

        fseek(f, 0, SEEK_END);
        uint table_offset = ftell(f);

        // data table to match idsoftware format
        uint table_count = ntohl(chunk_count);
        fwrite(&table_count, sizeof(uint32), 1, f);

        for (cc = 0; cc < chunk_count; cc++) {
            a_uncompressed_size[cc] = ntohl(a_uncompressed_size[cc]);
            a_compressed_size[cc] = ntohl(a_compressed_size[cc]);
            a_hash[cc] = ntohl(a_hash[cc]);
            fwrite(&a_uncompressed_size[cc], sizeof(uint32), 1, f);
            fwrite(&a_compressed_size[cc], sizeof(uint32), 1, f);
            fwrite(&a_hash[cc], sizeof(uint32), 1, f);
        }

        // footer
        size = ntohl(ftell(f));
        uint nullbytes = 0;
        fwrite(&nullbytes, sizeof(uint32), 1, f);
        fwrite(&size, sizeof(uint32), 1, f);

        // overwrite placeholder with table_offset
        fseek(f, magic, SEEK_SET);
        table_offset = ntohl(table_offset);
        fwrite(&table_offset, sizeof(uint32), 1, f);
        fclose(f);

    }
    
    printf("File compressed successfully.\n");
    return 0;
}

int main(int argc, char* argv[]) {
    printf("idSaveDecompressor v0.1 by SamPT\n");

    if (argc < 4) {
        printf("USAGE: idSaveDecompressor.exe [options] source dest\n");
        printf("Options:\n");
        printf("\t-c\tCompresses an uncompressed game_duration.dat file\n");
        printf("\t-d\tDecompresses a compressed game_duration.dat file\n");
        return 1;
    }

    struct stat sb;
    if (stat(argv[3], &sb) >= 0) {
        if (std::string(argv[2]) != std::string(argv[3])) {
            printf("Error: destination file already exists.\n\n");
            return 1;
        }
    } 

    std::string option = argv[1];
    int result = 0;

    if (option == "-c")
        result = compressSaveData(argv[2], argv[3], idchunksize);
    else if (option == "-d")
        result = decompressSaveData(argv[2], argv[3], idchunksize);
    else
        printf("Unknown option \"%s\"\n", argv[1]);
    
    // Catch for PS4 save being handled incorrectly by the .bat file. 
    if (result == 2)
        result = compressSaveData(argv[2], argv[3], idchunksize);

    return result;
}