书写清晰、规范的 Git Commit 记录是团队协作和项目管理的重要环节。以下是一些最佳实践和划分 Commit 的建议：

---

### 📌 **Commit 记录书写规范**

#### 1. **格式**

推荐使用以下格式：

```
<类型>(<范围>): <主题>

<正文>

<脚注>
```

- **类型**：说明 Commit 的性质（如 `feat`、`fix`、`docs` 等）。
- **范围**：可选，说明影响的范围（如模块、文件）。
- **主题**：简洁描述 Commit 的内容。
- **正文**：可选，详细说明变更的原因和影响。
- **脚注**：可选，用于引用问题单或关闭 Issue。

#### 2. **示例**

```plaintext
feat(user): 添加用户注册功能

- 新增用户注册 API
- 添加用户模型验证逻辑

Closes #123
```

---

### 📌 **Commit 类型**

以下是常见的 Commit 类型：

| **类型**   | **说明**                         | **示例**                          |
| ---------- | -------------------------------- | --------------------------------- |
| `feat`     | 新增功能                         | `feat(auth): 添加 OAuth 登录支持` |
| `fix`      | 修复 Bug                         | `fix(api): 修复分页参数错误`      |
| `docs`     | 文档更新                         | `docs(readme): 更新安装说明`      |
| `style`    | 代码样式调整（不影响功能）       | `style: 格式化代码`               |
| `refactor` | 代码重构（不新增功能或修复 Bug） | `refactor(utils): 优化日志输出`   |
| `test`     | 测试相关                         | `test(user): 添加用户模型测试`    |
| `chore`    | 构建或工具链变更                 | `chore: 更新依赖版本`             |
| `perf`     | 性能优化                         | `perf(db): 优化查询性能`          |
| `ci`       | CI/CD 配置变更                   | `ci: 添加 GitHub Actions 配置`    |
| `revert`   | 回滚 Commit                      | `revert: 回滚错误的提交`          |

---

### 📌 **Commit 划分原则**

#### 1. **单一职责原则**

每个 Commit 应只完成一个任务，避免混杂多个功能或修复。

- **错误示例**：
  ```plaintext
  feat: 添加用户注册和登录功能
  ```
- **正确示例**：
  ```plaintext
  feat(user): 添加用户注册功能
  feat(auth): 添加用户登录功能
  ```

#### 2. **原子性**

每个 Commit 应是独立的、可运行的单元，避免提交半成品。

- **错误示例**：
  ```plaintext
  feat: 用户注册功能（未完成）
  ```
- **正确示例**：
  ```plaintext
  feat(user): 添加用户注册 API
  feat(user): 添加用户注册前端页面
  ```

#### 3. **逻辑分组**

将相关的变更放在一个 Commit 中，避免拆分过于零散。

- **错误示例**：
  ```plaintext
  style: 格式化用户模型代码
  style: 格式化用户控制器代码
  ```
- **正确示例**：
  ```plaintext
  style(user): 格式化用户相关代码
  ```

#### 4. **小步提交**

频繁提交小规模的变更，便于代码审查和问题排查。

- **错误示例**：
  ```plaintext
  feat: 完成用户模块开发
  ```
- **正确示例**：
  ```plaintext
  feat(user): 添加用户模型
  feat(user): 添加用户注册 API
  feat(user): 添加用户登录 API
  ```

---

### 📌 **Commit 划分示例**

#### 场景：开发用户注册功能

1. **添加用户模型**：
   ```plaintext
   feat(user): 添加用户模型
   ```
2. **添加用户注册 API**：
   ```plaintext
   feat(user): 添加用户注册 API
   ```
3. **添加用户注册前端页面**：
   ```plaintext
   feat(user): 添加用户注册前端页面
   ```
4. **修复用户注册 Bug**：
   ```plaintext
   fix(user): 修复用户注册时邮箱验证问题
   ```
5. **更新文档**：
   ```plaintext
   docs(user): 更新用户注册 API 文档
   ```

---

### 📌 **工具支持**

1. **Commitizen**  
   一个交互式工具，帮助生成规范的 Commit 记录。

   ```bash
   npm install -g commitizen
   commitizen init cz-conventional-changelog --save-dev --save-exact
   ```

   使用：

   ```bash
   git cz
   ```

2. **Husky + Commitlint**  
   在提交时自动检查 Commit 格式。
   ```bash
   npm install husky @commitlint/cli @commitlint/config-conventional --save-dev
   ```
   配置：
   ```bash
   echo "module.exports = { extends: ['@commitlint/config-conventional'] }" > .commitlintrc.js
   npx husky add .husky/commit-msg 'npx --no -- commitlint --edit $1'
   ```

---

### 📌 **总结**

- **书写规范**：遵循 `<类型>(<范围>): <主题>` 格式。
- **划分原则**：单一职责、原子性、逻辑分组、小步提交。
- **工具支持**：使用 Commitizen、Husky 等工具提升效率。

通过规范的 Commit 记录和合理的划分，可以显著提升代码可读性和团队协作效率！
