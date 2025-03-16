import logging
from typing import Union


def get_logger(name: str, level: Union[str, int] = "INFO") -> logging.Logger:
    """
    Creates and returns a standardized logger with the given name and level.

    Args:
        name (str): The name of the logger (usually __name__).
        level (str|int): The logging level ('DEBUG', 'INFO', etc.) or logging constant.

    Returns:
        logging.Logger: Configured logger instance.
    """

    logger = logging.getLogger(name)

    # Prevent duplicate handlers
    if logger.hasHandlers():
        return logger

    # Format: [2025-03-16 23:59:59] [INFO] mymodule: Your message
    formatter = logging.Formatter(
        "[%(asctime)s] [%(levelname)s] %(name)s: %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )

    handler = logging.StreamHandler()
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    # Support both string and int levels
    if isinstance(level, str):
        level = level.upper()
        level = getattr(logging, level, logging.INFO)

    logger.setLevel(level)

    return logger
