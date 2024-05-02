#pragma once

#include <cassert>
#include <list>
#include <string>
#include <unordered_map>

#include "common/types.h"
#include "instruction.h"

#include "Allocator/UtilObjectBank.h"

namespace Shader::Gcn {

class GcnToken;
class GcnTokenList;

enum class GcnConditionOp : u32 {
    EqBool,
    NeBool,
    EqU32,
    NeU32,
    GeU32,
    GtU32,
    LeU32,
    LtU32,
    Scc0,
    Scc1,
    Vccz,
    Vccnz,
    Execz,
    Execnz,
    Divergence // Condition is (EXEC & SubgroupEqMask)
};

struct GcnTokenValue {
    size_t value;
    GcnScalarType type;
};

struct GcnTokenCondition {
    GcnConditionOp op;
    size_t cmpValue;
    GcnToken* condVar;
};

struct GcnTokenCode {
    size_t vertexId;            // vertex id in original CFG
    u32 pc;                     // program counter of this code block
    GcnInstructionList insList; // gcn instruction list
};

enum class GcnTokenKind : u32 {
    Invalid = 0,
    Code = 1 << 0,
    Loop = 1 << 1,
    Block = 1 << 2,
    If = 1 << 3,
    IfNot = 1 << 4,
    Else = 1 << 5,
    Branch = 1 << 6,
    End = 1 << 7,
    Variable = 1 << 8,
    SetValue = 1 << 9,
};

class GcnToken {
    friend class GcnTokenFactory;
    friend class GcnTokenList;

public:
    GcnToken(GcnTokenKind kind, GcnToken* match);

    GcnToken(GcnTokenKind kind, GcnTokenCode&& code);

    GcnToken(GcnTokenKind kind, const GcnTokenValue& value, GcnToken* match);

    GcnToken(GcnTokenKind kind, const GcnTokenCondition& cond, GcnToken* match);

    ~GcnToken();

    GcnTokenKind kind() const {
        return m_kind;
    }

    void setMatch(GcnToken* match) {
        m_match = match;
    }

    GcnToken* getMatch() const {
        return m_match;
    }

    const GcnTokenCondition& getCondition() const {
        return m_condition;
    }

    GcnTokenCondition& getCondition() {
        return m_condition;
    }

    const GcnTokenCode& getCode() const {
        return m_code;
    }

    GcnTokenCode& getCode() {
        return m_code;
    }

    const GcnTokenValue& getValue() const {
        return m_value;
    }

    GcnTokenValue& getValue() {
        return m_value;
    }

    std::list<GcnToken*>::iterator getIterator() const;

    GcnToken* getPrevNode() const;
    GcnToken* getNextNode() const;

    std::string dump() const;

    static GcnConditionOp getConditionOp(const GcnShaderInstruction& ins);
    static GcnConditionOp getInversePredicate(GcnConditionOp op);

private:
    GcnTokenKind m_kind;

    union {
        GcnTokenCode m_code;           // used by Code token
        GcnTokenCondition m_condition; // used by If/IfNot token
        GcnTokenValue m_value;         // used by Variable/SetValue token
    };

    // A related token, for example,
    // the match of a If token is End
    GcnToken* m_match;
    // Container holding this token
    GcnTokenList* m_container = nullptr;
};

class GcnTokenFactory {
public:
    GcnTokenFactory(util::ObjectBank<GcnToken>& tokenPool);
    ~GcnTokenFactory();

    GcnToken* createCode(GcnTokenCode&& code) {
        return m_pool.allocate(GcnTokenKind::Code, std::move(code));
    }

    GcnToken* createLoop() {
        return m_pool.allocate(GcnTokenKind::Loop, nullptr);
    }

    GcnToken* createLoopEnd(GcnToken* loopBegin) {
        assert(loopBegin && loopBegin->m_kind == GcnTokenKind::Loop);
        auto loopEnd = m_pool.allocate(GcnTokenKind::End, loopBegin);
        loopBegin->m_match = loopEnd;
        return loopEnd;
    }

    GcnToken* createBlock() {
        return m_pool.allocate(GcnTokenKind::Block, nullptr);
    }

    GcnToken* createBlockEnd(GcnToken* blockBegin) {
        assert(blockBegin && blockBegin->m_kind == GcnTokenKind::Block);
        auto blockEnd = m_pool.allocate(GcnTokenKind::End, blockBegin);
        blockBegin->m_match = blockEnd;
        return blockEnd;
    }

    GcnToken* createIf(GcnConditionOp op, u32 cmpValue = 0, GcnToken* variable = nullptr) {
        assert(!variable || variable->m_kind == GcnTokenKind::Variable);
        GcnTokenCondition condition = {};
        condition.op = op;
        condition.cmpValue = cmpValue;
        condition.condVar = variable;
        return m_pool.allocate(GcnTokenKind::If, condition, nullptr);
    }

    GcnToken* createIfNot(const GcnTokenCondition& condition) {
        return m_pool.allocate(GcnTokenKind::IfNot, condition, nullptr);
    }

    GcnToken* createElse(GcnToken* tokenIf) {
        assert(!tokenIf || tokenIf->m_kind == GcnTokenKind::If);
        auto tokenElse = m_pool.allocate(GcnTokenKind::Else, nullptr);
        if (tokenIf) {
            tokenIf->m_match = tokenElse;
        }
        return tokenElse;
    }

    GcnToken* createIfEnd(GcnToken* tokenIf, GcnToken* tokenElse) {
        assert(!tokenIf || tokenIf->m_kind == GcnTokenKind::If);
        assert(!tokenElse || tokenElse->m_kind == GcnTokenKind::Else);
        assert(tokenIf || tokenElse);

        auto tokenEnd = m_pool.allocate(GcnTokenKind::End, tokenIf);
        if (tokenElse) {
            tokenElse->m_match = tokenEnd;
        } else {
            tokenIf->m_match = tokenEnd;
        }

        return tokenEnd;
    }

    GcnToken* createBranch(GcnToken* target) {
        return m_pool.allocate(GcnTokenKind::Branch, target);
    }

    GcnToken* createVariable(GcnTokenValue init) {
        return m_pool.allocate(GcnTokenKind::Variable, init, nullptr);
    }

    GcnToken* createSetValue(GcnTokenValue value, GcnToken* variable) {
        assert(variable && variable->m_kind == GcnTokenKind::Variable);
        return m_pool.allocate(GcnTokenKind::SetValue, value, variable);
    }

private:
    util::ObjectBank<GcnToken>& m_pool;
};

class GcnTokenList {
    using TokenListType = std::list<GcnToken*>;

public:
    GcnTokenList();
    ~GcnTokenList();

public:
    /// Token iterators...
    typedef TokenListType::iterator iterator;
    typedef TokenListType::const_iterator const_iterator;
    typedef TokenListType::reverse_iterator reverse_iterator;
    typedef TokenListType::const_reverse_iterator const_reverse_iterator;

    /// Token iterator methods
    // clang-format off
		inline iterator                begin()       { return m_list.begin(); }
		inline const_iterator          begin() const { return m_list.begin(); }
		inline iterator                end  ()       { return m_list.end();   }
		inline const_iterator          end  () const { return m_list.end();   }

		inline reverse_iterator        rbegin()       { return m_list.rbegin(); }
		inline const_reverse_iterator  rbegin() const { return m_list.rbegin(); }
		inline reverse_iterator        rend  ()       { return m_list.rend();   }
		inline const_reverse_iterator  rend  () const { return m_list.rend();   }

		inline size_t                   size() const { return m_list.size();  }
		inline bool                    empty() const { return m_list.empty(); }
		inline const GcnToken      &front() const { return *m_list.front(); }
		inline       GcnToken      &front()       { return *m_list.front(); }
		inline const GcnToken       &back() const { return *m_list.back();  }
		inline       GcnToken       &back()       { return *m_list.back();  }
    // clang-format on

    iterator find(const GcnToken* token) {
        return m_iteratorMap[token];
    }

    void append(GcnToken* token) {
        token->m_container = this;
        m_list.push_back(token);
        m_iteratorMap[token] = std::prev(m_list.end());
    }

    iterator insert(iterator where, GcnToken* token) {
        token->m_container = this;
        auto iter = m_list.insert(where, token);
        m_iteratorMap[token] = iter;
        return iter;
    }

    iterator insertAfter(iterator where, GcnToken* token) {
        token->m_container = this;
        iterator iter = {};
        if (empty()) {
            iter = m_list.insert(begin(), token);
        } else {
            iter = m_list.insert(++where, token);
        }
        m_iteratorMap[token] = iter;
        return iter;
    }

    void moveAfter(iterator where, iterator first, iterator last) {
        m_list.splice(std::next(where), m_list, first, last);
    }

    void erase(GcnToken* token) {
        token->m_container = nullptr;
        const auto& iter = m_iteratorMap[token];
        m_list.erase(iter);
        m_iteratorMap.erase(token);
    }

    std::string dump(const GcnToken* target = nullptr) const;

private:
    std::list<GcnToken*> m_list;
    std::unordered_map<const GcnToken*, iterator> m_iteratorMap;
};

} // namespace Shader::Gcn
