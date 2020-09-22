#include "IKeyboard.h"
#include <cstring>
#include <memory>
#include <regex>

#include <Utils/Logger.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Input/KeyboardInputEvent.h>

#include <assert.h>

using yang::IKeyboard;

yang::IKeyboard::IKeyboard()
{
    memset(m_keyState, 0, sizeof(m_keyState));
    memset(m_previousKeyState, 0, sizeof(m_keyState));
}

bool yang::IKeyboard::Init()
{
    return true;
}

void yang::IKeyboard::SetKeyState(KeyCode key, bool isPressed)
{
    EventDispatcher* pDispatcher = EventDispatcher::Get();
    assert(pDispatcher);
    if (key != KeyCode::kMaxCodes)
    {
        m_keyState[static_cast<size_t>(key)] = isPressed;
        if (isPressed && m_previousKeyState[static_cast<size_t>(key)])
        {
            pDispatcher->TriggerEvent(std::make_unique<KeyboardInputEvent>(key, KeyboardInputEvent::EventType::kKeyDown));
        }
        else if (isPressed && !m_previousKeyState[static_cast<size_t>(key)])
        {
            pDispatcher->TriggerEvent(std::make_unique<KeyboardInputEvent>(key, KeyboardInputEvent::EventType::kKeyPressed));
        }
        else if (!isPressed)
        {
            pDispatcher->TriggerEvent(std::make_unique<KeyboardInputEvent>(key, KeyboardInputEvent::EventType::kKeyReleased));
        }
    }
}

bool yang::IKeyboard::IsKeyDown(KeyCode key)
{
    return m_keyState[static_cast<size_t>(key)];
}

bool yang::IKeyboard::IsKeyPressed(KeyCode key)
{
    return m_keyState[static_cast<size_t>(key)] && !m_previousKeyState[static_cast<size_t>(key)];
}

bool yang::IKeyboard::IsKeyReleased(KeyCode key)
{
    return m_keyState[static_cast<size_t>(key)] && m_previousKeyState[static_cast<size_t>(key)];
}

void yang::IKeyboard::NextFrame()
{
    assert(sizeof(m_keyState) == sizeof(m_previousKeyState));
    memcpy(m_previousKeyState, m_keyState, sizeof(m_keyState));
}

std::unique_ptr<IKeyboard> yang::IKeyboard::Create()
{
    return std::make_unique<IKeyboard>();
}

IKeyboard::KeyCode yang::IKeyboard::KeyCodeFromString(const std::string& data)
{
    if (data.size() == 1)
    {
        bool bIsNumber = data[0] >= '0' && data[0] <= '9';
        bool bIsUppercaseLetter = data[0] >= 'A' && data[0] <= 'Z';
        bool bIsLowercaseLetter = data[0] >= 'a' && data[0] <= 'z';
        if (bIsNumber)
        {
            return static_cast<KeyCode>(data[0] - '0' + static_cast<size_t>(KeyCode::k0));
        }
        else if (bIsUppercaseLetter)
        {
            return static_cast<KeyCode>(data[0] - 'A' + static_cast<size_t>(KeyCode::kA));
        }
        else if (bIsLowercaseLetter)
        {
            return static_cast<KeyCode>(data[0] - 'a' + static_cast<size_t>(KeyCode::kA));
        }
    }
    else
    {
        // F1 - F12
        std::regex re("f1[0-2]|f[1-9]", std::regex::egrep | std::regex::icase);
        if (std::regex_match(data, re))
        {
            return static_cast<KeyCode>(static_cast<size_t>(KeyCode::kF1) + std::stoul(data.substr(1)) - 1);
        }

        // LCTRL - RALT
        re.assign("(l|r)(ctrl|shift|alt)", std::regex::egrep | std::regex::icase);
        if (std::regex_match(data, re))
        {
            size_t code = tolower(data[0]) == 'l' ? 0 : 3;

            // if shift
            if (tolower(data[1]) == 's')
                ++code;
            // if alt
            else if (tolower(data[1]) == 'a')
                code += 2;

            return static_cast<KeyCode>(static_cast<size_t>(KeyCode::kLCTRL) + code);
        }

        // kRETURN - kSPACE
        re.assign("return|escape|backspace|tab|space", std::regex::egrep | std::regex::icase);
        if (std::regex_match(data, re))
        {
            size_t code = 0;

            // lazy fall-down switch case.
            switch (tolower(data[0]))
            {
            case 's':
                ++code;
            case 't':
                ++code;
            case 'b':
                ++code;
            case 'e':
                ++code;
            case 'r':
                return static_cast<KeyCode>(static_cast<size_t>(KeyCode::kRETURN) + code);
            default:
                return KeyCode::kMaxCodes;
            }
        }

        re.assign("right | left | down | up", std::regex::egrep | std::regex::icase);
        if (std::regex_match(data, re))
        {
            size_t code = 0;

            // lazy fall-down switch case.
            switch (tolower(data[0]))
            {
            case 'u':
                ++code;
            case 'd':
                ++code;
            case 'l':
                ++code;
            case 'r':
                return static_cast<KeyCode>(static_cast<size_t>(KeyCode::kRIGHT) + code);
            default:
                return KeyCode::kMaxCodes;
            }
        }
    }
    return KeyCode::kMaxCodes;
}
