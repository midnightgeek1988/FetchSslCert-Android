package ir.alireza.lib.opensslhandle

interface ConditionCheck {
    fun condition():Boolean
    fun finalyAssert()
}