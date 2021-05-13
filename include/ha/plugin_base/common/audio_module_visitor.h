// Copyright(c) 2021 Hansen Audio.

#pragma once

#include "kompositum/composite.h"
#include "kompositum/leaf.h"
#include "kompositum/visitor.h"

namespace ha {
namespace plugin_base {
namespace common {

//-----------------------------------------------------------------------------
/**
 * audio_module_visitor
 */
template <typename Func>
class audio_module_visitor : public Kompositum::Visitor
{
public:
    //-------------------------------------------------------------------------
    audio_module_visitor(Func func);

    void visit(Kompositum::Leaf* leaf) override;
    void visit(Kompositum::Composite* composite) override;

    //-------------------------------------------------------------------------
private:
    Func func;
};

//-----------------------------------------------------------------------------
template <typename Func>
audio_module_visitor<Func>::audio_module_visitor(Func func)
: func(std::move(func))
{
}

//-----------------------------------------------------------------------------
template <typename Func>
void audio_module_visitor<Func>::visit(Kompositum::Leaf* leaf)
{
    if (!leaf)
        return;

    func(leaf->getID(), false);
}

//-----------------------------------------------------------------------------
template <typename Func>
void audio_module_visitor<Func>::visit(Kompositum::Composite* composite)
{
    if (!composite)
        return;

    func(composite->getID(), true);

    if (composite->hasChildren())
    {
        composite->visitChildren(*this);
    }
}

//-----------------------------------------------------------------------------
} // namespace common
} // namespace plugin_base
} // namespace ha
