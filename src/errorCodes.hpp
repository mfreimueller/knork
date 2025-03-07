#pragma once

enum ErrorCode {
    None = 0,
    MalformedKnorkJson = 1,
    UnresolvedPlaceholder = 2,
    UnknownTemplate = 3,
    ExecutionError = 4,
    Unknown = 99
};