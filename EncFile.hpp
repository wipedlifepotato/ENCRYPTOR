#pragma once
#ifndef ENCFILE
#define ENCFILE
#include<fstream>
#include<filesystem>
#include<vector>
#include<cstring>
#include"ChaCha20.h"
static uint32_t CounterChaCha20 = 1;

class EncFile final{
 using mkeyT = std::vector<unsigned char>;
 protected:
    std::ofstream m_stream;
    std::string m_raw;
    std::filesystem::path m_path;
    mkeyT m_key, m_iv;
    void set( const std::string & data, mkeyT & to )
    {
        for(auto k : data)
        {
            to.push_back(k);
        }
    }
    void padding(mkeyT & t, size_t s)
    {
        auto copy = t;
        while(t.size() < s)
            t.insert(t.end(), copy.begin(), copy.end());
    }
 public:
    EncFile(const std::filesystem::path & path, std::string key, std::string iv, bool initNew = false, size_t bytes = 0): m_path{path} //: m_stream{path}
    {
        set_key(key);
        set_iv(iv);
        if (std::filesystem::exists(path))
        {
            if(initNew) {
                std::filesystem::path encPath = path;
                encPath += ".enc";
                m_stream = std::ofstream{encPath, std::ios::binary};
                //throw std::runtime_error("This file is exists");
                std::ifstream i{m_path, std::ios::binary};
                i.seekg(0, std::ios::end);
                std::string data(i.tellg(), '\0');
                i.seekg(0, std::ios::beg);
                i.read(data.data(), data.size());
                m_raw = data;
                m_stream << encrypt_chacha20(data);
            } else {
                std::filesystem::path encPath = path;
                encPath += ".dec";
                m_raw = encdecrypt();
                m_stream = std::ofstream{encPath,std::ios::binary};
                m_stream << m_raw;
            }
        }
        else if(initNew) {
            std::cerr << "Init new is not implemented idea, deprecated" << std::endl;
            m_stream = std::ofstream{path,std::ios::binary};
            for(size_t b = bytes;b--;)
            {
                m_stream << '\0';
            }
            m_raw="";
        } else {
            throw std::runtime_error("You will create a new file or select exists");
        }
    }
    void set_key(const std::string & data)
    {
        set(data, m_key);
    }
    void set_iv(const std::string & data)
    {
        set(data, m_iv);
    }

    std::string encrypt_chacha20(const std::string input, int block_size = 1024)
    {
        /*
         * The ChaCha20 stream cipher. The key length is 256 bits, the IV is 128 bits long. The first 32 bits consists of a counter in little-endian order followed by a 96 bit nonce. For example a nonce of:
         * */

        if(!m_key.size()) throw std::runtime_error("Set a key");
        if(!m_iv.size()) throw std::runtime_error("Set a iv");

        padding(m_key, 32);
        padding(m_iv, 12);

        unsigned char key[32];
        unsigned char iv[12];
        for(auto i = 0; i < sizeof(key); i++)
        {
            key[i] = m_key[i];
        }
        for(auto i = 0; i < sizeof(iv); i++)
        {
            iv[i] = m_iv[i];
        }
        // void Chacha20Init (Chacha20State& state, const uint8_t * nonce, const uint8_t * key, uint32_t counter)
        using namespace i2p::crypto::chacha;
        auto state = Chacha20State{};

        Chacha20Init(state, iv, key, CounterChaCha20);
        unsigned char tmp_buf[block_size];
        std::ostringstream o;
        memset(tmp_buf, 0, sizeof(tmp_buf));
        size_t c = 0;
        while (c < input.size())
        {
            auto len = std::min<size_t>(block_size, input.size() - c);
            auto p = input.substr(c, len);

            memset(tmp_buf, 0, sizeof(tmp_buf));
            memcpy(tmp_buf, p.data(), len);

            Chacha20Encrypt(state, tmp_buf, len);
            o.write(reinterpret_cast<char*>(tmp_buf), len);

            c += len;
        }
        return o.str();
    }
    EncFile & operator<<(const std::string & data)
    {
        if(!m_key.size() || !m_iv.size()) throw std::runtime_error("You need set a key and iv");
       // std::cout << "Input data: " << data << std::endl;
       // std::cout << "Input key[0]: " << m_key[0] << std::endl;;
       // std::cout << "Input nonce[0]: " << m_iv[0] << std::endl;;
        auto out = encrypt_chacha20(data);
        //m_stream << out << std::flush;
        m_stream.write(out.data(), out.size());
        m_stream.flush();
        return *this;
    }
    void operator>>(std::string data)
    {

    }
    std::string encdecrypt(void)
    {
        std::ifstream i{m_path, std::ios::binary};
        i.seekg(0, std::ios::end);
        std::string data(i.tellg(), '\0');
        i.seekg(0, std::ios::beg);
        i.read(data.data(), data.size());
        //std::cout << "Data: " << data << std::endl;
        return encrypt_chacha20(data);
    }
};
#endif
