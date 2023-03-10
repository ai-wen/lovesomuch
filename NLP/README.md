# 开源中文NLP工具和库：
性能 jieba

PaddleNLP：飞桨开源的自然语言处理库，包含中文分词、词性标注、命名实体识别、文本分类、机器翻译等功能。

HanLP：是由自然语言处理与社会计算实验室研制的中文自然语言处理工具包，涵盖了分词、词性标注、命名实体识别、关键词提取、依存句法分析、文本分类、情感分析等多个功能模块。

THULAC：是由清华大学自然语言处理与社会人文计算实验室研制的一款中文分词工具，具有高效、准确的特点。

jieba：是一款Python中文分词工具，采用了基于前缀词典实现的分词算法，具有高效、简单易用的特点。

SnowNLP：是由胡凯翔研究员开发的一款Python中文文本处理库，支持中文分词、情感分析、文本分类、关键词提取等多种功能。

FudanNLP：是由复旦大学自然语言处理实验室开发的中文自然语言处理工具包，支持分词、词性标注、命名实体识别、关键词提取、文本分类等多个功能。

NLPIR：是由北京中科红旗软件技术有限公司开发的中文自然语言处理工具包，支持分词、词性标注、命名实体识别、关键词提取、文本分类等多种功能。

BERT4keras：是一个基于Keras的自然语言处理工具，支持BERT模型、GPT模型等，可用于文本分类、情感分析、命名实体识别等多个任务。

transformers：是一个开源的自然语言处理工具库，提供了多种预训练模型，包括BERT、GPT等，支持文本分类、命名实体识别、情感分析、机器翻译等多个任务。

# 一些比较常用的指标和测试数据：
分词速度：分词是中文NLP中最基本的任务之一，分词速度是衡量NLP库性能的一个重要指标。常用的测试数据包括中文新闻、微博等语料库，测试分词效果和速度。

词性标注准确率：词性标注是将分词后的词汇标注上相应的词性，是中文NLP中的另一个基本任务。常用的测试数据包括人民日报语料库、CTB语料库等，测试词性标注的准确率。

命名实体识别准确率：命名实体识别是指从文本中识别出人名、地名、组织机构名等实体，是中文NLP中的一个比较难的任务。常用的测试数据包括人民日报语料库、MSRA语料库等，测试命名实体识别的准确率。

机器翻译BLEU指标：机器翻译是将一种语言的文本转化为另一种语言的文本，是中文NLP中的一个复杂的任务。BLEU指标是机器翻译领域中常用的评估指标之一，用于衡量机器翻译的准确度。常用的测试数据包括WMT、