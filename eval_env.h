#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include "./error.h"
#include "./builtins.h"
#include <unordered_map>
#include <algorithm>
#include <iterator>

class Value;
using ValuePtr = std::shared_ptr<Value>;

class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
private:
    std::unordered_map<std::string, ValuePtr> SYMBOLS;
    std::shared_ptr<EvalEnv> parent = nullptr;
    EvalEnv() {
        for (auto& proc : BUILTIN_PROCEDURES) {
            SYMBOLS.insert(std::make_pair(
                proc.first, std::make_shared<BuiltinProcValue>(proc.second)));
        }
    }
    EvalEnv(const std::unordered_map<std::string, ValuePtr>& SYMBOLS_,
            const std::shared_ptr<EvalEnv>& parent_) {
        SYMBOLS = SYMBOLS_;
        parent = parent_;
    }

public:
    static std::shared_ptr<EvalEnv> create() {
        return std::shared_ptr<EvalEnv>(new EvalEnv());
    }
    static std::shared_ptr<EvalEnv> create(
        const std::unordered_map<std::string, ValuePtr>& SYMBOLS_,
        const std::shared_ptr<EvalEnv>& parent_) {
        return std::shared_ptr<EvalEnv>(new EvalEnv(SYMBOLS_, parent_));
    }
    std::shared_ptr<EvalEnv> get_shared_this() {
        return shared_from_this();
    }
    std::vector<ValuePtr> evalList(const ValuePtr& expr);
    ValuePtr apply(const ValuePtr& proc, const std::vector<ValuePtr>& args);
    ValuePtr eval(const ValuePtr& expr);
    void defineBinding(const std::string& name, const ValuePtr& arg);
    ValuePtr lookupBinding(const std::string& name);
    std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& params, 
        const std::vector<ValuePtr>& args);
};

#endif
