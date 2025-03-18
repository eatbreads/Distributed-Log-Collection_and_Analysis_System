<script setup>
// 这里可以放置一些逻辑代码，如果需要的话
</script>

<template>
  <div>
    <h1>欢迎来到分布式日志收集与分析系统</h1>
    <section>
      <h2>项目介绍</h2>
      <p>
        本项目是一个分布式日志收集与分析系统，旨在提供高效的日志收集、存储、查询和分析功能。系统采用多种先进技术栈，确保高可用性和高性能。
      </p>
      <h3>涉及技术栈：</h3>
      <ul>
        <li><strong>C++</strong>（日志采集 Agent）</li>
        <li><strong>Kafka</strong>（日志流式处理）</li>
        <li><strong>Elasticsearch</strong>（日志存储和索引）</li>
        <li><strong>Redis</strong>（缓存查询）</li>
        <li><strong>Fluentd/Logstash</strong>（日志解析）</li>
        <li><strong>Prometheus + Grafana</strong>（可视化分析）</li>
        <li><strong>Docker + Kubernetes</strong>（分布式部署）</li>
      </ul>
      <h3>核心功能：</h3>
      <ul>
        <li>实时日志收集（从 Nginx、后端应用采集日志）</li>
        <li>日志解析和过滤（支持正则匹配）</li>
        <li>日志查询和分析（全文检索、聚合分析）</li>
        <li>日志告警（异常情况自动告警）</li>
      </ul>
      <h3>挑战点：</h3>
      <ul>
        <li>高效的日志传输（Kafka + ZeroMQ）</li>
        <li>分布式索引优化（ES 分片管理）</li>
        <li>实时日志流处理（Flink/Spark Streaming）</li>
      </ul>
    </section>
    <section>
      <h2>开发人员介绍</h2>
      <p>
        本项目由一支经验丰富的开发团队完成，团队成员在分布式系统、大数据处理和 DevOps 领域有着深厚的背景。
      </p>
      <h3>团队成员：</h3>
      <ul>
        <li><strong>小鸡</strong> - 负责日志采集 Agent 的开发与优化</li>
        <li><strong>小鸡</strong> - 负责 Kafka 和 Flink 的集成与流式处理</li>
        <li><strong>小鸡</strong> - 负责 Elasticsearch 的索引优化与存储管理</li>
        <li><strong>小鸡</strong> - 负责 Prometheus 和 Grafana 的可视化与监控</li>
        <li><strong>小鸡</strong> - 负责 Docker 和 Kubernetes 的部署与运维</li>
      </ul>
    </section>
  </div>
</template>

<style scoped>
/* 这里可以添加一些样式来美化页面 */
h1 {
  color: #2c3e50;
  text-align: center;
}

h2 {
  color: #34495e;
  border-bottom: 2px solid #34495e;
  padding-bottom: 10px;
}

h3 {
  color: #16a085;
}

ul {
  list-style-type: square;
  padding-left: 20px;
}

section {
  margin-bottom: 40px;
}
</style>