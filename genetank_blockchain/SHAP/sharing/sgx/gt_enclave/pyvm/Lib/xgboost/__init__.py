# coding: utf-8
"""XGBoost: eXtreme Gradient Boosting library.

Contributors: https://github.com/dmlc/xgboost/blob/master/CONTRIBUTORS.md
"""

from __future__ import absolute_import

import os

from .core import DMatrix, Booster
from .training import train, cv
#from . import rabit                   # noqa
try:
    from .sklearn import XGBModel, XGBClassifier, XGBRegressor, XGBRanker
    from .plotting import plot_importance, plot_tree, to_graphviz
except ImportError:
    pass

#VERSION_FILE = os.path.join(os.path.dirname(__file__), 'VERSION')
#with open(VERSION_FILE) as f:
__version__ = "1.1.0-SNAPSHOT"

__all__ = ['DMatrix', 'Booster',
           'train', 'cv',
           'XGBModel', 'XGBClassifier', 'XGBRegressor', 'XGBRanker',
           'plot_importance', 'plot_tree', 'to_graphviz']
