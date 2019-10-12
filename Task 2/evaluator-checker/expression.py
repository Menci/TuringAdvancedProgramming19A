from math import copysign

from utils import float_to_string

class Expression:
    def __init__(self, type, rng, value=None, operator=None, left_operand=None, right_operand=None):
        self.type = type
        self.value = value
        self.operator = operator
        self.left_operand = left_operand
        self.right_operand = right_operand
        self.rng = rng

    def evaluate(self):
        if self.type == "number":
            return self.value
        elif self.operator == "+":
            return self.left_operand.evaluate() + self.right_operand.evaluate()
        elif self.operator == "-":
            return self.left_operand.evaluate() - self.right_operand.evaluate()
        elif self.operator == "*":
            return self.left_operand.evaluate() * self.right_operand.evaluate()
        else:
            left_value = self.left_operand.evaluate()
            right_value = self.right_operand.evaluate()
            try:
                return left_value / right_value
            except ZeroDivisionError:
                if left_value == 0:
                    return float("nan")
                else:
                    return copysign(float("inf"), copysign(1, left_value) * copysign(1, right_value))
    
    def to_string(self):
        if self.type == "number":
            return float_to_string(self.value)
        format_string = [
            "(%s) %s (%s)",
            "(%s)%s (%s)",
            "(%s) %s(%s)",
            "(%s)%s(%s)",
        ][self.rng.randint(0, 3)]
        return format_string % (self.left_operand.to_string(), self.operator, self.right_operand.to_string())
