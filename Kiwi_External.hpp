//
//  Kiwi_External.h
//  Kiwi_External
//
//  Created by Pierre on 04/04/2018.
//  Copyright © 2018 Pierre. All rights reserved.
//

#pragma once

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
//#include <variant>

#ifdef _WIN32
#ifdef KIWI_LIBRARY_EXPORTS
#define KIWI_LIBRARY_EXTERN __declspec(dllexport)
#else
#define KIWI_LIBRARY_EXTERN __declspec(dllimport)
#endif
#else
#define KIWI_LIBRARY_EXTERN
#endif

namespace kiwi
{
    namespace external
    {
#ifdef KIWI_DSP_FLOAT
        typedef float sample_t;
#else
        typedef double sample_t;
#endif
        typedef std::runtime_error kerror_t;
        //typedef std::variant<std::string, double> atom_t;
        typedef std::vector<std::vector<sample_t>> buffer_t;
        // ==================================================================================== //
        //                                          OBJECT                                      //
        // ==================================================================================== //
        
        class Object
        {

        public: // methods

            //! @brief The constructor.
            //! @param ninputs The number of inputs.
            //! @param noutputs The number of outputs.
            inline constexpr Object(const size_t ninputs, const size_t noutputs) noexcept  :
            m_ninputs(ninputs), m_noutputs(noutputs) {}
            
            //! @brief The destructor.
            virtual ~Object() = default;
            
            //! @brief Gets the current number of inputs.
            inline size_t getNumberOfInputs() const noexcept {return m_ninputs;}
            
            //! @brief Gets the current number of outputs.
            inline size_t getNumberOfOutputs() const noexcept {return m_noutputs;}
            
            //! @brief Receives asynchonous messages.
            //! @details The method is used to communicate asynchronously with the object. If\n
            //! something is not valid, you should throw an errot_t.
            //! @param message The message.
            //virtual void receive(std::vector<atom_t> const& message) {}
            
            //! @brief Prepares everything for the perform method.
            //! @details You should use this method to check the vector size, the sample rate. If\n
            //! something is not valid, you should throw an errot_t.
            //! @param samplerate The sample rate.
            //! @param blocksize The number of samples per audio block.
            //! @see perform() and release()
            virtual void prepare(const size_t samplerate, const size_t blocksize) {}
            
            //! @brief Performs the digital signal processing.
            //! @details Triggers the callback set during prepare phase.
            //! @param input    The input audio buffer.
            //! @param output   The output audio buffer.
            //! @see prepare() and release()
            virtual void perform(buffer_t const& input, buffer_t& output) noexcept = 0;
            
            //! @brief Releases everything after the digital signal processing.
            //! @details You can use this method to free the memory allocated during the call of
            //! the prepare method for example.
            //! @see prepare() and perform()
            virtual void release() {}
            
        private: // members
            
            const size_t m_ninputs;
            const size_t m_noutputs;
        };
    }
}

typedef kiwi::external::Object *object_creator(void);
typedef void object_disposer(kiwi::external::Object *);

extern "C" KIWI_LIBRARY_EXTERN kiwi::external::Object* createObject();
extern "C" KIWI_LIBRARY_EXTERN void freeObject(kiwi::external::Object* c);

#define KIWI_LIBRARY_DECLARE(CLASSNAME)  \
extern "C" KIWI_LIBRARY_EXTERN kiwi::external::Object* create_object() { return new CLASSNAME(); } \
extern "C" KIWI_LIBRARY_EXTERN void free_object(kiwi::external::Object* c) { delete c; }

