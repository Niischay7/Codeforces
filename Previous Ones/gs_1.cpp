#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <cctype> // For isdigit, isspace
#include <limits>
#include <stdexcept> // For stoll, out_of_range, invalid_argument

using namespace std;

// --- Base64 Decoding and Utility Functions (from your provided code - first block) ---
string base64_decode(const string& encoded_string) {
    static const string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    static array<int, 256> base64_lookup = {}; // Zero-initialized
    static bool lookup_initialized = false;
    if (!lookup_initialized) {
        base64_lookup.fill(-1);
        for (std::string::size_type i = 0; i < base64_chars.length(); ++i)
{
            base64_lookup[static_cast<unsigned char>(base64_chars[i])] = i;
        }
        base64_lookup[static_cast<unsigned char>('=')] = -2; // Padding
        lookup_initialized = true;
    }

    string decoded_string;
    int in_len = encoded_string.size();
    int i = 0;
    array<int, 4> input_vals{}; // Initialize to zero
    int padding_count = 0;

    for (int in_ = 0; in_ < in_len; ++in_) {
        char c = encoded_string[in_];
        int val = -1;
        if (static_cast<unsigned char>(c) < 256) {
            val = base64_lookup[static_cast<unsigned char>(c)];
        }

        if (val == -1) return ""; // Invalid character
        if (val == -2) { // Padding character
            // Ensure padding is at the end
            // This logic seems to intend that padding chars must be in the last 1 or 2 positions of a 4-char block
            // and no non-padding chars follow padding chars.
            // A more robust check could be: if (padding_count > 0 && val != -2) return "";
            // And ensure total length is multiple of 4 unless is_valid_base64 is used first.
            // The existing code's padding logic is kept.
            if (in_ < in_len - 2 && (encoded_string[in_+1] != '=' || (in_ < in_len -1 && encoded_string[in_+2] != '=' && padding_count ==0 )  ) && encoded_string[in_len-1] != '='){
                 // A bit complex, basic idea: padding should only be at the end.
                 // This specific check is intricate, original logic kept.
            }
            padding_count++;
        } else {
            if (padding_count > 0) return ""; // Non-padding char after padding
            if (i >= 4) return ""; // Should not happen if logic is correct
            input_vals[i++] = val;
        }

        if (i == 4) {
            unsigned int block_bits = (input_vals[0] << 18) | (input_vals[1] << 12) | (input_vals[2] << 6) | input_vals[3];
            decoded_string += static_cast<char>((block_bits >> 16) & 0xFF);
            decoded_string += static_cast<char>((block_bits >> 8) & 0xFF);
            decoded_string += static_cast<char>(block_bits & 0xFF);
            i = 0;
            input_vals.fill(0);
        }
    }

    // Handle trailing bytes
    if (i > 0) {
        // Fill remaining input_vals with 0 for bit manipulation if not already
        for(int j = i; j < 4; ++j) input_vals[j] = 0;

        unsigned int block_bits = (input_vals[0] << 18) | (input_vals[1] << 12) | (input_vals[2] << 6) | input_vals[3];
        if (i == 1) return ""; // 1 base64 char (6 bits) is not enough to form a byte. (e.g. "A===" is invalid by some strict decoders)
                               // Standard encoding produces pairs of base64 chars for single byte output "XX=="
        
        decoded_string += static_cast<char>((block_bits >> 16) & 0xFF);
        if (i == 3) { // 3 base64 chars (18 bits) -> 2 bytes
            decoded_string += static_cast<char>((block_bits >> 8) & 0xFF);
        }
        // if i == 2 (e.g. "XX=="), only 1 byte is produced (already added).
        // if i == 4, it would have been processed in the loop.
    }
    
    // Final validation check based on expected output size
    // Each 4 Base64 chars (24 bits) decode to 3 bytes. Padding chars don't contribute bits.
    size_t expected_bits_from_base64 = (static_cast<size_t>(in_len) - padding_count) * 6;
    size_t actual_bits_in_decoded = decoded_string.size() * 8;

    if (padding_count > 0) { // Adjust for standard padding rules
        if (padding_count == 1 && i != 3) return ""; // "XXX=" means 3 data chars
        if (padding_count == 2 && i != 2) return ""; // "XX==" means 2 data chars
    }


    if (actual_bits_in_decoded / 8 * 8 != actual_bits_in_decoded) {} // Should be fine as it's size * 8
    if (expected_bits_from_base64 / 6 * 6 != expected_bits_from_base64) {} // Should be fine.

    // The number of useful bits must align. E.g. 12 bits from base64 (2 chars) -> 8 bits output (1 byte)
    // 18 bits from base64 (3 chars) -> 16 bits output (2 bytes)
    bool length_ok = false;
    if (padding_count == 0 && (expected_bits_from_base64 % 24 == 0) && (actual_bits_in_decoded == expected_bits_from_base64 / 24 * 24) ) length_ok = true;
    else if (padding_count == 1 && (expected_bits_from_base64 % 24 == 18) && (actual_bits_in_decoded == expected_bits_from_base64 / 24 * 24 + 16) ) length_ok = true;
    else if (padding_count == 2 && (expected_bits_from_base64 % 24 == 12) && (actual_bits_in_decoded == expected_bits_from_base64 / 24 * 24 + 8) ) length_ok = true;
    // Simplified check
    if (padding_count > 2) return ""; // Max 2 padding chars for a block of 4.
    if ( (in_len % 4 != 0) && padding_count == 0 && in_len > 0 ) return ""; // If no padding, length must be multiple of 4 (unless empty)
                                                                          // However, some decoders are more lenient if overall bit count is valid.
                                                                          // The original check was:
    // if (decoded_string.size() * 8 != (static_cast<size_t>(in_len) - padding_count) * 6) return "";
    // This checks if total bits match. This is a strong check.

    if ( (static_cast<size_t>(in_len) - padding_count) * 6 != decoded_string.size() * 8) {
        // Check if it's off by a few bits that would be truncated for the last byte(s)
        size_t data_chars = static_cast<size_t>(in_len) - padding_count;
        if (data_chars == 2) { // XX== -> 1 byte
            if (decoded_string.size() != 1) return "";
        } else if (data_chars == 3) { // XXX= -> 2 bytes
            if (decoded_string.size() != 2) return "";
        } else if (data_chars > 0 && data_chars % 4 != 0) { // Any other non-block aligned without padding is tricky
            return ""; 
        } else if (data_chars > 0 && (data_chars * 6 != decoded_string.size() * 8) ) {
            return "";
        }
    }


    return decoded_string;
}

bool is_valid_base64(const string& s) {
    static const string base64_chars_with_padding = "ABCDEFGHIJKLMNOPQRSTUVWXYZ""abcdefghijklmnopqrstuvwxyz""0123456789+/=";
    if (s.empty()) return true;
    size_t len = s.length();
    if (len % 4 != 0) return false; // Base64 string length must be a multiple of 4
    size_t padding_pos = string::npos;
    int padding_count = 0;
    for (size_t i = 0; i < len; ++i) {
        char c = s[i];
        if (base64_chars_with_padding.find(c) == string::npos) return false; // Invalid char
        if (c == '=') {
            if (padding_pos == string::npos) padding_pos = i; // First padding char
            padding_count++;
        } else {
            if (padding_pos != string::npos) return false; // Char after padding
        }
    }
    if (padding_count > 2) return false; // Max 2 padding chars
    // If padding exists, it must be at the end.
    // padding_pos would be len - padding_count if padding is contiguous at the end.
    if (padding_count > 0 && padding_pos != len - padding_count) return false;
    // Already checked by len % 4: if (padding_count > 0 && len < 4) return false;
    return true;
}

int bitmap_extraction(const string& base64_data) {
    string decoded_data = base64_decode(base64_data);
    if (decoded_data.empty()) return 0;
    string pattern_Start = "ABC{";
    string pattern_End = "}";
    size_t start_pos = decoded_data.find(pattern_Start);
    if (start_pos == string::npos) return 0;
    start_pos += pattern_Start.length();
    size_t end_pos = decoded_data.find(pattern_End, start_pos);
    if (end_pos == string::npos || end_pos <= start_pos) return 0;
    string number_str = decoded_data.substr(start_pos, end_pos - start_pos);
    for (char c : number_str) {
        if (!isdigit(static_cast<unsigned char>(c))) return 0;
    }
    if (number_str.empty()) return 0;
    try {
        unsigned long long number = stoull(number_str);
        if (number > static_cast<unsigned long long>(numeric_limits<int>::max())) return 0;
        return static_cast<int>(number);
    } catch (...) {
        return 0;
    }
}

int packet_pattern_extraction(const string& base64_data) {
    string decoded_data = base64_decode(base64_data);
    if (decoded_data.empty()) return 0;
    string pattern_Start = "ABC{";
    string pattern_End = "}";
    size_t start_pos = decoded_data.find(pattern_Start);
    if (start_pos == string::npos) return 0;
    start_pos += pattern_Start.length();
    size_t end_pos = decoded_data.find(pattern_End, start_pos);
    if (end_pos == string::npos || end_pos <= start_pos) return 0;
    string number_str = decoded_data.substr(start_pos, end_pos - start_pos);
    for (char c : number_str) {
        if (!isdigit(static_cast<unsigned char>(c))) return 0;
    }
    if (number_str.empty()) return 0;
    unsigned int result = 0;
    unsigned int modulus = 10007;
    for (char digit_char : number_str) {
        int digit = digit_char - '0';
        result = static_cast<unsigned int>((static_cast<unsigned long long>(result) * 10 + digit) % modulus);
    }
    result = (result + 3); // No modulo here in original, so result can exceed modulus
    return static_cast<int>(result);
}

uint8_t mystic_sim(const string& base64_bytecode, uint8_t memory_address_to_read) {
    string decoded_str = base64_decode(base64_bytecode);
    if (decoded_str.empty()) return 0;
    vector<uint8_t> bytecode(decoded_str.begin(), decoded_str.end());
    array<uint8_t, 16> registers;
    registers.fill(0);
    array<uint8_t, 256> memory;
    memory.fill(0);
    uint8_t instruction_pointer = 0;
    bool z_flag = false; // Set but potentially not used by all implemented opcodes
    registers[15] = 255; // Initialize R15
    const int MAX_INSTRUCTIONS = 1000000;
    int instruction_count = 0;

    while (static_cast<size_t>(instruction_pointer) < bytecode.size() && instruction_count < MAX_INSTRUCTIONS) {
        instruction_count++;
        uint8_t opcode = bytecode[instruction_pointer];
        if (opcode == 0xFF) break; // HALT instruction
        uint8_t instruction_len = 0;
        uint8_t operand1 = 0;
        uint8_t operand2 = 0;
        switch (opcode) {
            case 0x01: instruction_len = 3; break; // MOV imm, Ry
            case 0x02: instruction_len = 3; break; // ADD Rx, Ry
            case 0x03: instruction_len = 3; break; // SUB Rx, Ry
            case 0x04: instruction_len = 2; break; // JZ rel_offset (example, needs z_flag)
            case 0x05: instruction_len = 2; break; // JMP rel_offset (example)
            case 0x06: instruction_len = 3; break; // LD [Rx], Ry (example: Load from mem pointed by Rx into Ry)
            case 0x07: instruction_len = 3; break; // ST Rx, [Ry] (example: Store Rx into mem pointed by Ry)
            case 0x08: instruction_len = 2; break; // INC Rx (example)
            case 0x09: instruction_len = 1; break; // NOP (example)
            default: goto end_sim; // Unknown opcode
        }
        if (static_cast<size_t>(instruction_pointer) + instruction_len > bytecode.size()) goto end_sim; // Instruction out of bounds
        if (instruction_len > 1) operand1 = bytecode[instruction_pointer + 1];
        if (instruction_len > 2) operand2 = bytecode[instruction_pointer + 2];
        
        uint8_t next_instruction_pointer = instruction_pointer + instruction_len;

        switch (opcode) {
            case 0x01: { // MOV imm8, Ry
                uint8_t imm8 = operand1;
                uint8_t reg_y_idx = operand2 & 0x0F;
                if (reg_y_idx >= 16) goto end_sim;
                registers[reg_y_idx] = imm8;
                break;
            }
            case 0x02: { // ADD Rx, Ry (Ry = Ry + Rx)
                uint8_t reg_x_idx = operand1 & 0x0F;
                uint8_t reg_y_idx = operand2 & 0x0F;
                if (reg_x_idx >= 16 || reg_y_idx >= 16) goto end_sim;
                registers[reg_y_idx] = registers[reg_y_idx] + registers[reg_x_idx]; // Overflow wraps around (uint8_t)
                break;
            }
            case 0x03: { // SUB Rx, Ry (Ry = Ry - Rx)
                uint8_t reg_x_idx = operand1 & 0x0F;
                uint8_t reg_y_idx = operand2 & 0x0F;
                if (reg_x_idx >= 16 || reg_y_idx >= 16) goto end_sim;
                uint8_t result = registers[reg_y_idx] - registers[reg_x_idx]; // Underflow wraps around
                z_flag = (result == 0);
                registers[reg_y_idx] = result;
                break;
            }
            // --- Placeholder for other instructions (0x04 - 0x09) ---
            // case 0x04: // JZ rel_offset
            //    if (z_flag) {
            //        // rel_offset is signed for jumps. operand1 is uint8_t.
            //        int8_t offset = static_cast<int8_t>(operand1);
            //        next_instruction_pointer = instruction_pointer + offset; // careful with IP update logic
            //    }
            //    break;
            // Note: Implement 0x04-0x09 based on their actual specification.
            // The current code only defines their length. If they are not implemented,
            // they will fall through and IP will just advance by instruction_len.
        }
        instruction_pointer = next_instruction_pointer;
    }
end_sim:
    if (memory_address_to_read < memory.size()) {
        return memory[memory_address_to_read];
    }
    return 0; // Default if address out of bounds or error
}


// --- Manual JSON Parsing Helper Functions ---
void skip_json_whitespace(const string& s, size_t& pos) {
    while (pos < s.length() && isspace(s[pos])) {
        pos++;
    }
}

bool expect_json_char(const string& s, size_t& pos, char expected) {
    skip_json_whitespace(s, pos);
    if (pos < s.length() && s[pos] == expected) {
        pos++;
        return true;
    }
    return false;
}

bool expect_json_key(const string& s, size_t& pos, const string& key_name) {
    // Expects "key_name":
    if (!expect_json_char(s, pos, '"')) return false;
    if (s.substr(pos, key_name.length()) == key_name) {
        pos += key_name.length();
    } else {
        return false; // Key name mismatch
    }
    if (!expect_json_char(s, pos, '"')) return false;
    if (!expect_json_char(s, pos, ':')) return false;
    return true;
}

long long parse_json_long(const string& s, size_t& pos, bool& success) {
    skip_json_whitespace(s, pos);
    size_t original_pos = pos;
    string num_str;
    bool is_negative = false;
    if (pos < s.length() && s[pos] == '-') {
        is_negative = true;
        pos++;
    }
    size_t start_num = pos;
    while (pos < s.length() && isdigit(s[pos])) {
        num_str += s[pos];
        pos++;
    }
    if (num_str.empty()) { // No digits found after optional '-'
        success = false;
        pos = original_pos; // backtrack
        return 0;
    }
    // num_str already captured digits, no need to substr from s again
    if (is_negative) num_str = "-" + num_str;

    try {
        long long val = stoll(num_str);
        success = true;
        return val;
    } catch (...) {
        success = false;
        pos = original_pos; // backtrack
        return 0;
    }
}

string parse_json_string(const string& s, size_t& pos, bool& success) {
    skip_json_whitespace(s, pos);
    size_t original_pos = pos;

    if (pos >= s.length() || s[pos] != '"') {
        success = false;
        pos = original_pos;
        return "";
    }
    pos++; // Skip opening quote

    string value;
    bool escaped = false;
    while (pos < s.length()) {
        if (escaped) {
            // Basic escapes. Base64 itself won't contain these troublesome chars.
            if (s[pos] == '"' || s[pos] == '\\' || s[pos] == '/') value += s[pos];
            else if (s[pos] == 'b') value += '\b';
            else if (s[pos] == 'f') value += '\f';
            else if (s[pos] == 'n') value += '\n';
            else if (s[pos] == 'r') value += '\r';
            else if (s[pos] == 't') value += '\t';
            // \uXXXX not handled for simplicity for HackerRank
            else { success = false; pos = original_pos; return "";} // Invalid escape for this simple parser
            escaped = false;
        } else if (s[pos] == '\\') {
            escaped = true;
        } else if (s[pos] == '"') {
            pos++; // Skip closing quote
            success = true;
            return value;
        } else {
            value += s[pos];
        }
        pos++;
    }
    success = false; // No closing quote found
    pos = original_pos;
    return "";
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input_base64;
    getline(cin, input_base64);

    string json_str = base64_decode(input_base64);
    if (json_str.empty() && !input_base64.empty()) { // If base64_decode failed for non-empty input
        cout << "0 0 0\n";
        return 0;
    }
     if (json_str.empty() && input_base64.empty()) { // If input itself was empty, and base64 resulted in empty.
        // Depending on problem spec, this might be valid (e.g. n=0) or invalid.
        // The constraints n < 1 || n > 100 will catch n=0 later if it parses.
        // If an empty JSON string leads to n=0, it might still be "valid" up to a point.
        // For now, assume non-empty JSON if base64 was non-empty.
    }


    size_t pos = 0;
    bool parse_success;

    if (!expect_json_char(json_str, pos, '{')) { cout << "0 0 0\n"; return 0; }

    // Parse "n"
    if (!expect_json_key(json_str, pos, "n")) { cout << "0 0 0\n"; return 0; }
    long long n_long = parse_json_long(json_str, pos, parse_success);
    if (!parse_success || n_long < 1 || n_long > 100) { cout << "0 0 0\n"; return 0; }
    int n = static_cast<int>(n_long);

    if (!expect_json_char(json_str, pos, ',')) { cout << "0 0 0\n"; return 0; }

    // Parse "data"
    if (!expect_json_key(json_str, pos, "data")) { cout << "0 0 0\n"; return 0; }
    if (!expect_json_char(json_str, pos, '[')) { cout << "0 0 0\n"; return 0; }

    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            if (!expect_json_char(json_str, pos, ',')) { cout << "0 0 0\n"; return 0; }
        }
        if (!expect_json_char(json_str, pos, '{')) { cout << "0 0 0\n"; return 0; }

        // Parse "pcap"
        if (!expect_json_key(json_str, pos, "pcap")) { cout << "0 0 0\n"; return 0; }
        string pcap_b64 = parse_json_string(json_str, pos, parse_success);
        if (!parse_success) { cout << "0 0 0\n"; return 0; }

        if (!expect_json_char(json_str, pos, ',')) { cout << "0 0 0\n"; return 0; }

        // Parse "myst"
        if (!expect_json_key(json_str, pos, "myst")) { cout << "0 0 0\n"; return 0; }
        string myst_b64 = parse_json_string(json_str, pos, parse_success);
        if (!parse_success) { cout << "0 0 0\n"; return 0; }

        if (!expect_json_char(json_str, pos, ',')) { cout << "0 0 0\n"; return 0; }

        // Parse "memory_address"
        if (!expect_json_key(json_str, pos, "memory_address")) { cout << "0 0 0\n"; return 0; }
        long long mem_addr_long = parse_json_long(json_str, pos, parse_success);
        if (!parse_success || mem_addr_long < 0 || mem_addr_long > 255) { cout << "0 0 0\n"; return 0; }
        int memory_address = static_cast<int>(mem_addr_long);
        
        if (!expect_json_char(json_str, pos, '}')) { cout << "0 0 0\n"; return 0; }

        // --- Call processing functions ---
        int pcap_val = packet_pattern_extraction(pcap_b64);
        int bitmap_val = bitmap_extraction(pcap_b64); 
        uint8_t myst_val_uint8 = mystic_sim(myst_b64, static_cast<uint8_t>(memory_address));
        
        cout << pcap_val << ' ' << bitmap_val << ' ' << static_cast<int>(myst_val_uint8) << '\n';
    }

    if (!expect_json_char(json_str, pos, ']')) { cout << "0 0 0\n"; return 0; }
    if (!expect_json_char(json_str, pos, '}')) { cout << "0 0 0\n"; return 0; }
    
    skip_json_whitespace(json_str, pos);
    if (pos < json_str.length()) { // Check for trailing characters
        cout << "0 0 0\n"; 
        return 0;
    }

    return 0;
}
