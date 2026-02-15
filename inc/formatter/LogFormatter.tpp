


template <typename Policy>
LogFormatter<Policy>::LogFormatter(std::string app, std::string ctx) 
    : app_name(std::move(app)), context(std::move(ctx)) {}

template <typename Policy>
std::string LogFormatter<Policy>::msgDescription(float val) const {
    return context + " usage: " + std::to_string(val) + std::string(Policy::unit);
}

template <typename Policy>
std::optional<LogMessage> LogFormatter<Policy>::formatDataToLogMsg(const std::string& rawData) {
    try {
        float value = std::stof(rawData);
        SeverityLvl_enum severity = Policy::inferSeverity(value);

        return LogMessage(
            app_name,
            context,
            mapToLogType(severity),
            std::chrono::system_clock::now(),
            msgDescription(value)
        );
    } 
    catch (...) {
        return std::nullopt;
    }
}